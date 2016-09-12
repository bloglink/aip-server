#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTime>
#include <QObject>
#include <QTcpServer>

#include "TcpSocket.h"

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);

protected:
    void incomingConnection(qintptr handle);

signals:
    void SendMsg(quint16 port, QByteArray msg);
public slots:
    void AllQuit(void);
    void TcpQuit(quint16);
    void RecvMsg(quint16 port, QByteArray msg);
public:
    QList<TcpSocket *> Pool;
    QList<quint16 > Port;
};

#endif // TCPSERVER_H
