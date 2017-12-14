#ifndef SQLSERVER_H
#define SQLSERVER_H

#include <QTime>
#include <QObject>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QDebug>

#define SQL "./data/aip-server.db"

class SqlServer : public QObject
{
    Q_OBJECT
public:
    explicit SqlServer(QObject *parent = 0);

public slots:
    void Init(void);
    void Quit(void);
    void InsertState(QStringList msg);
    void InsertTrust(QString msg);
    void InsertUsers(QString msg);
    QStringList ListUsers(void);
    QStringList ListTrust(void);
public:
    QSqlDatabase db;
};

#endif // SQLSERVER_H
