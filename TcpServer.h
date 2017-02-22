#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);

protected:
    void incomingConnection(qintptr handle);
signals:
    void NewGuest(quint16 handle);
};

#endif // TCPSERVER_H
