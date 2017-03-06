#ifndef SQLSERVER_H
#define SQLSERVER_H

#include <QTime>
#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QDebug>

#include "define.h"

#define SQL "./data/aip-server.db"

class SqlServer : public QObject
{
    Q_OBJECT
public:
    explicit SqlServer(QObject *parent = 0);
signals:
    void SendMessage(TcpMap map,QByteArray msg);
public slots:
    void Init(void);
    void Quit(void);
    void ReadMessage(TcpMap map,QByteArray msg);
    void GuestRecord(TcpMap map,QByteArray msg);
    void GuestStatus(TcpMap map,QByteArray msg);
    void GuestRemove(TcpMap map,QByteArray msg);
    void GuestLogin(TcpMap map,QByteArray msg);
public:
    QSqlDatabase db;
};

#endif // SQLSERVER_H
