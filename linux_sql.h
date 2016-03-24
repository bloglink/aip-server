#ifndef LINUX_SQL_H
#define LINUX_SQL_H

#include <QWidget>
#include <QFile>
#include <QDebug>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>

class LINUX_SQL : public QWidget
{
    Q_OBJECT
public:
    explicit LINUX_SQL(QWidget *parent = 0);
    
public:
    QSqlDatabase db;
public slots:
    int MaxID(QVariant table);
    bool ConnectToSql(QVariant sql);
    bool DisConnect();
    bool IsExist(QVariant sql);
};

#endif // LINUX_SQL_H
