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
    void ExcuteCmd(quint16 addr,quint16 cmd,QByteArray data);
public slots:
    void AllQuit(void);
    void TransformCmd(quint16 addr0,quint16 addr1,quint16 cmd,QByteArray data);
public:
    QList<TcpSocket *> Pool;
    QList<quint16 > Port;
};

#endif // TCPSERVER_H
