#ifndef SQLSERVER_H
#define SQLSERVER_H

#include <QDir>
#include <QFile>
#include <QTime>
#include <QUuid>
#include <QDebug>
#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QJsonObject>
#include <QSqlDatabase>

#define SQL_PATH "./nandflash/"
#define SQL "./nandflash/server.db"

class SqlServer : public QObject
{
    Q_OBJECT
public:
    explicit SqlServer(QObject *parent = 0);
public slots:
    void Init(void);
    void Insert(QJsonObject json);
private:
    QSqlDatabase db;
};

#endif // SQLSERVER_H
