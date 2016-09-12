#ifndef SQLSERVER_H
#define SQLSERVER_H

#include <QTime>
#include <QObject>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>

#define SQL "./aip-server.db"

class SqlServer : public QObject
{
    Q_OBJECT
public:
    explicit SqlServer(QObject *parent = 0);

public slots:
    void SqlOpen(QString name);
    void SqlClose(void);
    void SqlInsert(QString msg);
    void SqlCreate(QString msg);
    bool SqlIsExist(QString msg);
    int SqlMaxCount(QString msg);
public:
    QSqlDatabase db;
};

#endif // SQLSERVER_H
