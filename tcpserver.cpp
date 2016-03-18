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

    emit ClientConnect(handle, client->peerAddress().toString(),client->peerPort());

    ClientList.append(client);
    ClientID.append(handle);
    ClientHeart.append(0);
    clientCount++;

    CurrentClient = client;
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
    CurrentClient->write(data);
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
    for (i=0; i<clientCount; i++)
        ClientList[i]->write(data);
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
    int heart = 0xfe;
    int port;
    QString ip;
    for (i=0; i<clientCount; i++) {
        if (ClientHeart[i] != 0)
            SendData(ClientID[i],QString::number(heart).toUtf8());
        ClientHeart[i]++;
        if (ClientHeart[i] > 3) {
            port = ClientList[i]->peerPort();
            ip = ClientList[i]->peerAddress().toString();
            ClientList[i]->disconnectFromHost();
        }
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
            ClientHeart[i] = 0;
    }

}
