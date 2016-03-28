/*******************************************************************************
 * Copyright (c) 2016,青岛艾普智能仪器有限公司
 * All rights reserved.
 *
 * version:     1.0
 * author:      link
 * date:        2016.03.18
 * brief:       服务端
*******************************************************************************/
#include "tcpserver.h"
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.18
  * brief:      构造函数
******************************************************************************/
tcpServer::tcpServer(QObject *parent) :
    QTcpServer(parent)
{
    int i;

    for (i=0; i<MAX_CLIENT; i++) {
        tcpPool[i] = new tcpClient(this);
        tcpPool[i]->Info.isFree = true;
        tcpPool[i]->Info.ID = QString::number(i);

        connect(tcpPool[i], SIGNAL(RcvData(int,QByteArray)),
                this, SIGNAL(ClientRcvData(int,QByteArray)));
        connect(tcpPool[i], SIGNAL(ClientDisConnect(int)),
                this, SLOT(DisConnect(int)));
    }
    ClientCount = 0;
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.18
  * brief:      新的连接
******************************************************************************/
void tcpServer::incomingConnection(int handle)
{
    int i;

    for (i=0; i<MAX_CLIENT; i++) {
        if (tcpPool[i]->Info.isFree == true) {
            tcpPool[i]->Info.isFree =false;
            break;
        }
    }

    if (!tcpPool[i]->setSocketDescriptor(handle)) {
        qDebug()<<tcpPool[i]->error();
        return;
    }
    tcpPool[i]->Info.TIME = QTime::currentTime().toString();
    tcpPool[i]->Info.isInit = false;
    tcpPool[i]->Info.heart = 0;
    ClientList.append(tcpPool[i]);
    ClientID.append(i);
    ClientCount++;

    CurrentClient = tcpPool[i];

    emit ClientConnected(i);
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.18
  * brief:      移除指定连接
******************************************************************************/
void tcpServer::DisConnect(int index)
{
    int i;
    for (i=0; i<ClientCount; i++) {
        if (ClientID[i] == index) {
            ClientID.removeAt(i);
            ClientList.removeAt(i);
            ClientCount--;
            i--;
            emit ClientDisconnect(index);
            break;
        }
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.18
  * brief:      指定客户端连接发消息
******************************************************************************/
void tcpServer::SendData(int index, QByteArray data)
{
    if (ClientCount<1)
        return;

    QByteArray msg;
    QDataStream out(&msg,QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<(qint64)0<<(quint8)0;
    out<<data;
    out.device()->seek(0);
    out<<(qint64)(msg.size()-sizeof(qint64))<<(quint8)(send_type_msg);

    tcpPool[index]->write(msg);
//    tcpPool[index]->waitForBytesWritten(-1);
}


/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.18
  * brief:      关闭所有连接
******************************************************************************/
void tcpServer::CloseAllClient()
{
    int i;
    for (i=0; i<ClientCount; i++) {
        ClientList[i]->close();
        i--;
    }
}

