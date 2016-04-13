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
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.22
  * brief:      客户端
******************************************************************************/
tcpClient::tcpClient(QObject *parent,int ClientIndex) :
    QTcpSocket(parent)
{
    index        = ClientIndex;
    heart        = 0;
    loadSize     = 4*1024;
    blockSize    = 0;
    totalBytes   = 0;
    bytesToWrite = 0;
    bytesWritten = 0;

    connect(this, SIGNAL(readyRead()), this, SLOT(ReadData()));
    connect(this, SIGNAL(disconnected()), this, SLOT(DisConnect()));
    connect(this, SIGNAL(bytesWritten(qint64)),
            this, SLOT(updateClientProgress(qint64)));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(displayError(QAbstractSocket::SocketError)));
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.22
  * brief:      读取数据
******************************************************************************/
void tcpClient::ReadData()
{
    qint64 type;
    qint64 target;
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_8);

    while (1) {
        if (blockSize == 0) {
            if (this->bytesAvailable() < sizeof(qint64))
                return;
            in >> blockSize;
        }

        if (blockSize > 32*1024) {
            this->abort();
            return;
        }

        if (this->bytesAvailable() < blockSize)
            return;

        msg.clear();
        in >> type >> target >> msg;

        emit RcvMessage(index, type ,target, msg);

        blockSize = 0;
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.29
  * brief:      发送消息
******************************************************************************/
void tcpClient::SendMessage(quint64 type, quint64 origin, QByteArray data)
{
    int ret;
    msg.clear();
    QDataStream out(&msg, QIODevice::ReadWrite);

    out.setVersion(QDataStream::Qt_4_8);
    out<<(qint64)0 << (quint64)type << (quint64)origin << data;
    out.device()->seek(0);
    out<<(qint64)(msg.size()-sizeof(qint64));

    if (this->state() != QAbstractSocket::ConnectedState)
        return;
    ret = this->writeData(msg, msg.size());
    if (ret == -1)
        qDebug() << "write data error!" << this->errorString();
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.29
  * brief:      准备发送文件
******************************************************************************/
void tcpClient::StartTransfer(QString name)
{
    file = new QFile(name);
    if (!file->open(QFile::ReadOnly)) {
        qDebug() << "open file error!" << file->errorString();
        return;
    }
    msg.clear();
    msg = QCryptographicHash::hash(file->readAll(),QCryptographicHash::Md5);
    this->SendMessage(send_md5, serverIndex, msg);

    file->seek(0);

    totalBytes = file->size();
    msg.clear();
    msg.append(QString::number(totalBytes));
    this->SendMessage(send_size, serverIndex, msg);

    QString currentName = name.right(name.size()- name.lastIndexOf('/')-1);
    msg.clear();
    msg.append(currentName.toUtf8());

    this->SendMessage(send_name, serverIndex, msg);

    bytesToWrite = totalBytes;
    bytesWritten = 0;
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.29
  * brief:      发送文件内容
******************************************************************************/
void tcpClient::updateClientProgress(qint64 numBytes)
{
    if (bytesToWrite == 0)
        return;

    bytesWritten += (int)numBytes;

    msg.clear();
    msg = file->read(loadSize);

    this->SendMessage(send_file, serverIndex, msg);

    bytesToWrite -= (int)qMin(bytesToWrite,loadSize);

    if (bytesToWrite == 0)
        file->close();

}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.29
  * brief:      心跳处理
******************************************************************************/
void tcpClient::HeartBeat()
{
    heart++;
    if (heart >= 5 && (Info.isInit == false)) {
        heart = 0;
        disconnectFromHost();
    }
    if (heart >= 100) {
        heart = 0;
        disconnectFromHost();
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.29
  * brief:      清零心跳
******************************************************************************/
void tcpClient::HeartClear()
{
    heart = 0;
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.22
  * brief:      断开连接
******************************************************************************/
void tcpClient::DisConnect()
{
    emit ClientDisConnect(index);
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.22
  * brief:      输出错误信息
******************************************************************************/
void tcpClient::displayError(QAbstractSocket::SocketError)
{
    qDebug()<<this->errorString(); //输出错误信息
}
