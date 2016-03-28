/*******************************************************************************
 * Copyright (c) 2016,青岛艾普智能仪器有限公司
 * All rights reserved.
 *
 * version:     1.0
 * author:      link
 * date:        2016.03.22
 * brief:       客户端
*******************************************************************************/
#include "tcpclient.h"

tcpClient::tcpClient(QObject *parent) :
    QTcpSocket(parent)
{
    blockSize = 0;
    loadSize = 4*1024;
    totalBytes = 0;

    bytesToWrite = 0;
    bytesWritten = 0;
    connect(this, SIGNAL(readyRead()), this, SLOT(ReadData()));
    connect(this, SIGNAL(disconnected()), this, SLOT(DisConnect()));
    connect(this,SIGNAL(bytesWritten(qint64)),this,SLOT(updateClientProgress(qint64)));
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.22
  * brief:      读取数据
******************************************************************************/
void tcpClient::ReadData()
{
    quint8 fun;
    QByteArray data;
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_8);

    while (1) {
        if (blockSize == 0) {
            if (this->bytesAvailable() < sizeof(qint64))
                return;
        }
        in >> blockSize;
        if (this->bytesAvailable() < blockSize)
            return;

        in >> fun;

        switch (fun) {
        case reply_type_msg:
            in >> data;
            emit RcvData(Info.ID.toInt(), data);
            break;
        case send_type_file:
            break;
        default:
            break;
        }

        blockSize = 0;
    }
}
void tcpClient::SendFile(QByteArray data)
{
    QByteArray msg;
    QDataStream out(&msg,QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<(qint64)0<<(quint8)0;
    out<<data;
    out.device()->seek(0);
    out<<(qint64)(msg.size()-sizeof(qint64))<<(quint8)(send_type_file);

    this->write(msg);
}
void tcpClient::SendMessage(quint8 type, QByteArray data)
{
    int ret;
    QByteArray msg;
    QDataStream out(&msg,QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<(qint64)0<<(quint8)0<<data;
    out.device()->seek(0);
    out<<(qint64)(msg.size()-sizeof(qint64))<<(quint8)(type);
    ret = this->writeData(msg, msg.size());
    if (ret == -1)
        qDebug()<<this->errorString();
}

void tcpClient::startTransfer()
{
    QByteArray msg;
    QString fileName = "/home/link/aip-server/001.cpp";
    localFile = new QFile(fileName);
    if(!localFile->open(QFile::ReadOnly))
    {
        qDebug() << "open file error!";
        return;
    }


    totalBytes = localFile->size();
    msg.append(QString::number(totalBytes));
    this->SendMessage(send_type_size, msg);

    QString currentFileName = fileName.right(fileName.size()
                                             - fileName.lastIndexOf('/')-1);
    msg.clear();
    msg.append(currentFileName.toUtf8());

    this->SendMessage(send_type_name, msg);

    bytesToWrite = totalBytes;
    bytesWritten = 0;

}

void tcpClient::updateClientProgress(qint64 numBytes)  //发送文件内容
{
    if (bytesToWrite == 0)
        return;

    QByteArray msg;

    bytesWritten += (int)numBytes;

    msg = localFile->read(loadSize);

    this->SendMessage(send_type_file, msg);

    bytesToWrite -= (int)qMin(bytesToWrite,loadSize);
    qDebug()<<bytesToWrite;

    if (bytesToWrite == 0)
        localFile->close();

}

/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.22
  * brief:      断开连接
******************************************************************************/
void tcpClient::DisConnect()
{
    emit ClientDisConnect(Info.ID.toInt());
}
