#ifndef SQLSERVER_H
#define SQLSERVER_H

#include <QUrl>
#include <QTime>
#include <QObject>
#include <QVariant>
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
    void TransformCmd(QUrl url);
public slots:
    void Init(void);
    void Quit(void);
    void InsertState(QUrl url);
    void UsersCheck(QUrl url);
public:
    QSqlDatabase db;
};

#endif // SQLSERVER_H
