#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QWidget>
#include <QTcpServer>
#include <tcpclient.h>
#include <QMessageBox>
#include <QTimer>
#include <QDataStream>
class tcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit tcpServer(QObject *parent = 0);
    
public:
    QList<int> ClientID;
    QList<tcpClient *> ClientList;
    QList<tcpClient *> ClientActive;
    tcpClient *CurrentClient;

    tcpClient *tcpPool[MAX_CLIENT];

    int ClientCount;
public slots:
    void SendData(int clientID, QByteArray data);
    void SendDataCurrent(QByteArray data);
    void SendDataAll(QByteArray data);

    void CloseAllClient();


protected:
    void incomingConnection(int handle);

signals:
    void ClientConnected(int index);
    void ClientDisconnect(int index);
    void ClientRcvData(int index, QByteArray data);
    void error(QTcpSocket::SocketError socketError);

private slots:
    void DisConnect(int index);
};

#endif // TCPSERVER_H
