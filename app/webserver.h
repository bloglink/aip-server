/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       服务器
*******************************************************************************/
#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <QTimer>
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>

#include "tcpserver.h"
#include "tcpsocket.h"
#include "sqlite.h"

class WebServer : public QObject
{
    Q_OBJECT
public:
    explicit WebServer(QObject *parent = 0);

signals:

public slots:
    void initServer();
private slots:
    void newClient(quint16 qrt);
    void newMaster(quint16 qrt);
    void newOnline(QByteArray msg);
    void newSignin(QByteArray msg);
    void disClient();
    void disMaster();
    void display(QString msg);
    void recvClient(quint16 addr, quint16 cmd, QByteArray msg);
    void recvMaster(quint16 addr, quint16 cmd, QByteArray msg);
    void initSql();
    void check();
private:
    TcpServer *tcp;
    TcpServer *usr;
    Sqlite *sql;
    QTimer *timer;

    QMap<int, TcpSocket*> clients;
    QMap<int, TcpSocket*> masters;
    QMap<int, QJsonObject> objs;
};

#endif // WEBSERVER_H
