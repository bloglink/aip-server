#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QTcpSocket>
#include <myhelper.h>
#include <QHostAddress>
class tcpClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit tcpClient(QObject *parent = 0,int clientID=0);
    
private:
    int clientID;

signals:
    void ClientReadData(int clientID,QString IP,int Port,QByteArray data);
    void ClientDisConnect(int clientID,QString IP,int Port);

private slots:
    void ReadData();
    void DisConnect();

public slots:
    
};

#endif // TCPCLIENT_H
