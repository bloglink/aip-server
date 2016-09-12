#include "TcpSocket.h"

TcpSocket::TcpSocket(QObject *parent) : QTcpSocket(parent)
{
    loadSize     = 4*1024;
    blockSize    = 0;
    bytesToWrite = 0;
    bytesWritten = 0;

    connect(this,SIGNAL(readyRead()), this, SLOT(ReadData()));
    connect(this,SIGNAL(disconnected()),this,SLOT(TcpDisc()));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(displayError(QAbstractSocket::SocketError)));
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      客户退出
******************************************************************************/
void TcpSocket::TcpDisc()
{
    emit TcpQuit(this->peerPort());
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      读取数据
******************************************************************************/
void TcpSocket::ReadData()
{
    QByteArray msg;
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_8);

    while (1) {
        if (blockSize == 0) {
            if (this->bytesAvailable() < qint64(sizeof(qint64)))
                return;
            in >> blockSize;
        }
        if (blockSize > 8*1024) {
            qDebug() << "block size overflow";
            this->abort();
            return;
        }
        if (this->bytesAvailable() < blockSize)
            return;
        in >> msg;
        blockSize = 0;
        ExcuteCmd(msg);
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      命令执行
******************************************************************************/
void TcpSocket::ExcuteCmd(QByteArray msg)
{
    switch (msg.at(0)) {
    case 'B'://初始化
        msg.insert(2,QString("%1 ").arg(QTime::currentTime().toString()));
        msg.insert(2,QString("%1 ").arg(peerAddress().toString()));
        msg.insert(2,QString("%1 ").arg(peerPort()));
        emit SendMsg(this->peerPort(),msg);
        break;
    case 'H'://心跳
        WriteMsg("H");
        break;
    default:
        emit SendMsg(this->peerPort(),msg);
        break;
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      写数据
******************************************************************************/
void TcpSocket::WriteMsg(QByteArray data)
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);

    out.setVersion(QDataStream::Qt_4_8);
    out<<(qint64)0 << data;
    out.device()->seek(0);
    out<<(qint64)(msg.size()-sizeof(qint64));

    this->writeData(msg, msg.size());
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      输出错误信息
******************************************************************************/
void TcpSocket::displayError(QAbstractSocket::SocketError socketError)
{
    if (socketError == QAbstractSocket::RemoteHostClosedError)
        return;
    qDebug()<<"error:"<<this->errorString(); //输出错误信息
    this->close();
}
