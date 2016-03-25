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
    connect(this,SIGNAL(readyRead()),this,SLOT(ReadData()));
    connect(this,SIGNAL(disconnected()),this,SLOT(DisConnect()));
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.22
  * brief:      读取数据
******************************************************************************/
void tcpClient::ReadData()
{
    QByteArray data;
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_8);

    while (1) {
        if (blockSize == 0) {
            if (this->bytesAvailable() < sizeof(quint16))
                return;
        }
        in >> blockSize;
        if (this->bytesAvailable() < blockSize)
            return;

        in >> data;

        emit RcvData(Info.ID.toInt(), data);

        blockSize = 0;
    }
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
