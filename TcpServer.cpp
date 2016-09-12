/*******************************************************************************
 * Copyright (c) 2016,青岛艾普智能仪器有限公司
 * All rights reserved.
 *
 * version:     1.0
 * author:      link
 * date:        2016.03.18
 * brief:       服务程序
*******************************************************************************/
#include "TcpServer.h"

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{
    Pool.clear();
    Port.clear();
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.18
  * brief:      新的连接
******************************************************************************/
void TcpServer::incomingConnection(qintptr handle)
{
    Pool.append(new TcpSocket(this));
    if (Pool.last()->setSocketDescriptor(handle)) {
        qDebug()<<"new tcp" << QTime::currentTime().toString()<<Pool.size()<<Pool.last()->peerPort();
        Port.append(Pool.last()->peerPort());
        connect(Pool.last(),
                SIGNAL(TcpQuit(quint16)),
                this,
                SLOT(TcpQuit(quint16)));
        connect(Pool.last(),
                SIGNAL(SendMsg(quint16,QByteArray)),
                this,
                SLOT(RecvMsg(quint16,QByteArray)));
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.18
  * brief:      关闭所有连接
******************************************************************************/
void TcpServer::AllQuit()
{
    for (int i=0; i<Pool.size(); i++) {
        Pool.at(i)->close();
    }
    this->close();
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.18
  * brief:      连接断开
******************************************************************************/
void TcpServer::TcpQuit(quint16 port)
{
    for (int i=0; i<Pool.size(); i++) {
        if (Pool.at(i)->peerPort() == port) {
            Pool.removeAt(i);
            Port.removeAt(i);
            emit SendMsg(port,"Q");
        }
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.18
  * brief:      接收数据
******************************************************************************/
void TcpServer::RecvMsg(quint16 port, QByteArray msg)
{
    if (msg.at(0) == 'T') {
        int o = 0;
        int t = 0;
        quint16 tp = quint16(msg.remove(0,2).toInt());
        for (int i=0; i<Port.size(); i++) {
            if (Port.at(i) == port) {
                o = i;
                break;
            }
        }
        for (int i=0; i<Port.size(); i++) {
            if (Port.at(i) == tp) {
                t = i;
                break;
            }
        }
        Port[o] = tp;
        Port[t] = port;
        return;
    }

    for (int i=0; i<Port.size(); i++) {
        if (Port.at(i) == port) {
            if (Pool.at(i)->peerPort() == port)
                emit SendMsg(port,msg);
            else
                Pool.at(i)->WriteMsg(msg);
        }
    }
}
/*******************************************************************************
 *                                  END
*******************************************************************************/
