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

    for (i=0; i<max_client; i++) {
        tcpPool[i] = new tcpClient(this,i);
        tcpPool[i]->Info.isFree = true;

        connect(tcpPool[i], SIGNAL(RcvMessage(int,quint64,quint64,QByteArray)),
                this, SIGNAL(ClientRcvMessage(int,quint64,quint64,QByteArray)));
        connect(tcpPool[i], SIGNAL(ClientDisConnect(int)),
                this, SLOT(DisConnect(int)));
    }
    currentIndex = 0;
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

    for (i=currentIndex; i<max_client; i++) {
        if (tcpPool[i]->Info.isFree == true) {
            index = i;
            break;
        }
    }
    currentIndex++;
    if (currentIndex >= max_client)
        currentIndex = 0;

    if (!tcpPool[index]->setSocketDescriptor(handle)) {
        qDebug() << "initialize error!" << tcpPool[index]->errorString();
        return;
    }
    tcpPool[index]->HeartClear();

    tcpPool[index]->Info.Time = QTime::currentTime().toString();
    tcpPool[index]->Info.Address   = tcpPool[i]->peerAddress().toString();
    tcpPool[index]->Info.isFree =false;
    tcpPool[index]->Info.isInit = false;
    ClientIndex.append(index);

    emit ClientConnected(index);
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
    for (i=0; i<ClientIndex.size(); i++) {
        if (ClientIndex[i] == index) {
            ClientIndex.removeAt(i);
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
    for (i=0; i<ClientIndex.size(); i++) {
        index = ClientIndex[i];
        tcpPool[index]->close();
        i--;
    }
}

