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

        connect(tcpPool[i], SIGNAL(RcvMessage(int,quint8,QByteArray)),
                this, SIGNAL(ClientRcvMessage(int,quint8,QByteArray)));
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
    ClientIndex.append(i);
    ClientCount++;

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
        if (ClientIndex[i] == index) {
            ClientIndex.removeAt(i);
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
  * brief:      关闭所有连接
******************************************************************************/
void tcpServer::CloseAllClient()
{
    int i;
    int index;
    for (i=0; i<ClientCount; i++) {
        index = ClientIndex[i];
        tcpPool[index]->close();
        i--;
    }
}

