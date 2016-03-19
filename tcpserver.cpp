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

    connect(client,SIGNAL(ClientReadData(int,QString,int,QByteArray)),
            this,SIGNAL(ClientReadData(int,QString,int,QByteArray)));
    connect(client,SIGNAL(ClientDisConnect(int,QString,int)),
            this,SLOT(DisConnect(int,QString,int)));
    connect(client,SIGNAL(updateShow()),this,SIGNAL(updateShow()));

    emit ClientConnect(handle, client->peerAddress().toString(),client->peerPort());

    ClientList.append(client);
    ClientID.append(handle);
    clientCount++;

    CurrentClient = client;
    client->Info.Time = QTime::currentTime().toString();
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
void tcpServer::DisConnect(int clientID,QString IP,int Port)
{
    int i;
    for (i=0; i<clientCount; i++) {
        if (ClientID[i] == clientID) {
            ClientList[i]->close();
            ClientID.removeAt(i);
            ClientList.removeAt(i);
            ClientHeart.removeAt(i);
            clientCount--;
            i--;
            emit ClientDisConnect(clientID,IP,Port);
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
    QByteArray msg;
    msg[0] = quint8(sendtype_msg);
    msg.append(data);
    for (int i=0;i<clientCount;i++) {
        if (ClientID[i] == clientID) {
            ClientList[i]->write(msg);
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
  * brief:      对所有连接发送心跳包
******************************************************************************/
void tcpServer::SendHeartBeat(int clientID)
{
    int i;
    QByteArray msg;
    msg[0] = quint8(type_heart);

    for (i=0;i<clientCount;i++) {
        if (ClientID[i] == clientID) {
            ClientList[i]->write(msg);
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
    int port;
    QString ip;

    for (i=0; i<clientCount; i++) {
        if (ClientList[i]->clientHeart > 5) {
            port = ClientList[i]->peerPort();
            ip = ClientList[i]->peerAddress().toString();
            ClientList[i]->disconnectFromHost();
        }
        if (ClientList[i]->clientHeart == 0)
            SendHeartBeat(ClientID[i]);
        ClientList[i]->clientHeart++;
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.18
  * brief:      心跳清零
******************************************************************************/
void tcpServer::clearHeart(int temp)
{
    int i;
    for (i=0; i<clientCount; i++) {
        if (ClientID[i] == temp)
            ClientList[i]->clientHeart = 0;
    }
}

