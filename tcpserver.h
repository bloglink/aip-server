#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QDataStream>

#include "tcpclient.h"

class tcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit tcpServer(QObject *parent = 0);
    
public:
    QList<int> ClientID;
    QList<tcpClient *> ClientList;

    tcpClient *CurrentClient;

    tcpClient *tcpPool[MAX_CLIENT];

    int ClientCount;
public slots:
    void SendData(int index, QByteArray data);
    void SendDataCurrent(QByteArray data);
    void SendDataAll(QByteArray data);

    void CloseAllClient();

protected:
    void incomingConnection(int handle);

signals:
    void ClientConnected(int index);
    void ClientDisconnect(int index);
    void ClientRcvData(int index, QByteArray data);

private slots:
    void DisConnect(int index);
};

#endif // TCPSERVER_H
