#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QWidget>
#include <QTcpServer>
#include <tcpclient.h>
#include <QMessageBox>
class tcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit tcpServer(QObject *parent = 0);
    


public:
    QList<tcpClient *> ClientList;
    QList<int> ClientID;
    tcpClient *CurrentClient;

    int clientCount;
public slots:
    void SendData(int clientID, QByteArray data);
    void SendDataCurrent(QByteArray data);
    void SendDataAll(QByteArray data);

    int ClientCount()const{return clientCount;}
    void CloseAllClient();

protected:
    void incomingConnection(int handle);

signals:
    void error(QTcpSocket::SocketError socketError);
    void ClientReadData(int clientID,QString IP,int Port,QByteArray data);
    void ClientConnect(int clientID,QString IP,int Port);
    void ClientDisConnect(int clientID,QString IP,int Port);

private slots:
    void DisConnect(int clientID,QString IP,int Port);
    
};

#endif // TCPSERVER_H
