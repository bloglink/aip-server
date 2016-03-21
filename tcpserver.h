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
    void updateShow();
    void newRecord(QString No,int state);
    void shareData(QByteArray data);
    void error(QTcpSocket::SocketError socketError);

private slots:
    void DisConnect(int clientID);
    void heartBeat();
    
};

#endif // TCPSERVER_H
