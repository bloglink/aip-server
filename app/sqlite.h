/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       数据库
*******************************************************************************/
#ifndef SQLSERVER_H
#define SQLSERVER_H

#include <QTime>
#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QDebug>

#include <QJsonObject>

#include "lqsnowflake.h"

class Sqlite : public QObject
{
    Q_OBJECT
public:
    explicit Sqlite(QObject *parent = 0);

public slots:
    void connect(void);
    void appendOnline(QJsonObject msg);
    void deleteOnline(QJsonObject msg);
    bool checkMaster(QJsonObject msg);
public:
    QSqlDatabase db;
    LQSnowflake snow;
};

#endif // SQLSERVER_H
