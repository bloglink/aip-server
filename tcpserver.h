#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTime>
#include <QTcpServer>

#include "tcpclient.h"

class tcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit tcpServer(QObject *parent = 0);

signals:
    void ClientConnected(int index);
    void ClientDisconnect(int index);
    void ClientRcvMessage(int index, quint8 type, QByteArray data);
public:
    int ClientCount;
    QList<int> ClientIndex;
    tcpClient *tcpPool[MAX_CLIENT];
public slots:
    void CloseAllClient();
protected:
    void incomingConnection(int handle);
private slots:
    void DisConnect(int index);
};

#endif // TCPSERVER_H
