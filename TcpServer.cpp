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
        qDebug()<<"new tcp" << QTime::currentTime().toString();
        Port.append(Pool.last()->peerPort());
        connect(Pool.last(),
                SIGNAL(TransformCmd(quint16,quint16,quint16,QByteArray)),
                this,
                SLOT(TransformCmd(quint16,quint16,quint16,QByteArray)));
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
  * brief:      转发数据
******************************************************************************/
void TcpServer::TransformCmd(quint16 addr0,quint16 addr1,quint16 cmd,QByteArray data)
{
    if (addr1 == ADDR) {
        emit ExcuteCmd(addr0,cmd,data);
     } else {
        for (int i=0; i<Pool.size(); i++) {
            if (Pool.at(i)->peerPort() == addr1) {
                Pool.at(i)->TransmitAddr = addr0;
                if (cmd == ServerGetHead)
                    Pool.at(i)->PutBlock(ADDR,cmd,data);
                else
                    Pool.at(i)->PutBlock(addr0,cmd,data);
            }
        }
    }
}
/*******************************************************************************
 *                                  END
*******************************************************************************/
