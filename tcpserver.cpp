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
    this->clientCount=0;

    QTimer *timer = new QTimer(this);
    connect (timer,SIGNAL(timeout()),this,SLOT(heartBeat()));
    timer->start(5000);
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.18
  * brief:      新的连接
******************************************************************************/
void tcpServer::incomingConnection(int handle)
{
    tcpClient *client = new tcpClient(this,handle);

    if (!client->setSocketDescriptor(handle)) {
        emit error(client->error());
        return;
    }

    connect(client,SIGNAL(shareData(QByteArray)),
            this,SIGNAL(shareData(QByteArray)));
    connect(client,SIGNAL(updateShow()),this,SIGNAL(updateShow()));
    connect(client,SIGNAL(newRecord(QString,int)),
            this,SIGNAL(newRecord(QString,int)));
    connect(client,SIGNAL(ClientDisConnect(int)),
            this,SLOT(DisConnect(int)));
    ClientList.append(client);
    ClientID.append(handle);
    clientCount++;

    CurrentClient = client;
    client->Info.Id = QString::number(handle);
    client->Info.Port = QString::number(client->peerPort());
    client->Info.Time = QTime::currentTime().toString();
    client->Info.heart = 0;

    emit updateShow();

    QByteArray data;
    data[0] = type_ip;
    client->write(data);
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.18
  * brief:      移除指定连接
******************************************************************************/
void tcpServer::DisConnect(int clientID)
{
    int i;
    for (i=0; i<clientCount; i++) {
        if (ClientID[i] == clientID) {
            ClientList[i]->close();
            ClientID.removeAt(i);
            ClientList.removeAt(i);
            clientCount--;
            i--;
            emit updateShow();
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
void tcpServer::SendData(int clientID, QByteArray data)
{
    for (int i=0;i<clientCount;i++) {
        if (ClientID[i] == clientID) {
            ClientList[i]->write(data);
            break;
        }
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.18
  * brief:      对当前连接发送数据
******************************************************************************/
void tcpServer::SendDataCurrent(QByteArray data)
{
    if (clientCount<1)
        return;
    QByteArray msg;
    msg[0] = quint8(sendtype_msg);
    msg.append(data);

    CurrentClient->write(msg);
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.18
  * brief:      对所有连接发送数据
******************************************************************************/
void tcpServer::SendDataAll(QByteArray data)
{
    int i;
    QByteArray msg;
    msg[0] = quint8(sendtype_msg);
    msg.append(data);
    for (i=0; i<clientCount; i++)
        ClientList[i]->write(msg);
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
    for (i=0; i<clientCount; i++) {
        ClientList[i]->close();
        i--;
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.18
  * brief:      心跳检测
******************************************************************************/
void tcpServer::heartBeat()
{
    int i;

    QByteArray msg;
    msg[0] = quint8(type_heart);

    for (i=0; i<clientCount; i++) {
        if (ClientList[i]->clientHeart > 5) {
            ClientList[i]->disconnectFromHost();
        }else if (ClientList[i]->clientHeart == 0) {
            SendData(ClientID[i],msg);
        }
        ClientList[i]->clientHeart++;
    }
}


