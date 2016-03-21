#ifndef LINUX_SQL_H
#define LINUX_SQL_H

#include <QWidget>
///////////////////////////////////////////////////////////////////////////////
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QSqlError>
#include <QFile>
///////////////////////////////////////////////////////////////////////////////
class LINUX_SQL : public QWidget
{
    Q_OBJECT
public:
    explicit LINUX_SQL(QWidget *parent = 0);
    
public:
    QSqlDatabase db;
public slots:
    int selectMax(QVariant table);
    bool createConnetion(QVariant sql);
    bool close_connection();
    bool isExist(QVariant sql);
};

#endif // LINUX_SQL_H
