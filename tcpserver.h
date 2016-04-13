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
    void ClientRcvMessage(int index, quint64 type, quint64 target, QByteArray data);
public:
    QList<int> ClientIndex;
    tcpClient *tcpPool[max_client];
public slots:
    void CloseAllClient();
protected:
    void incomingConnection(int handle);
private slots:
    void DisConnect(int index);
private:
    int pos;
    int index;
    int currentIndex;
};

#endif // TCPSERVER_H
