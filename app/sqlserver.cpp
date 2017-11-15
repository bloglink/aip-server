/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       数据库
*******************************************************************************/
#include "sqlserver.h"

SqlServer::SqlServer(QObject *parent) : QObject(parent)
{
}

void SqlServer::connect()
{
    db = QSqlDatabase::addDatabase("QMYSQL3", "mysql");
    db.setHostName("192.168.1.55");
    db.setUserName("root");
    db.setPassword("87973318");
    db.setDatabaseName("aip-server");
    if (!db.open()) {
        qDebug() << "open fail" << db.lastError();
    } else {
        QSqlQuery query(db);
        query.exec("delete from aip_online");
    }
}

void SqlServer::appendOnline(QJsonObject msg)
{
    double recUuid = snow.getId();
    double devUuid = snow.getId();
    QString devAddr = msg.value("devAddr").toString();
    QString devNumb = msg.value("devNumb").toString();
    QString netPort = QString::number(msg.value("netPort").toInt());
    QString netAddr = msg.value("netAddr").toString();
    QString recTime = msg.value("recTime").toString();
    QString netStat = msg.value("netStat").toString();
    QString verNumb = msg.value("verNumb").toString();

    QSqlQuery query(db);

    query.prepare("select devUuid from aip_device where devAddr = :devAddr");
    query.bindValue(":devAddr", devAddr);
    if (!query.exec()) {
        qDebug() << query.lastError();
    }
    if (query.next()) {
        devUuid = query.value(0).toDouble();
    } else {
        query.prepare("insert into aip_device values(?,?,?,?,?,?,?)");
        query.bindValue(0, devUuid);
        query.bindValue(1, devAddr);
        query.bindValue(2, devNumb);
        query.bindValue(3, "--");
        query.bindValue(4, "--");
        query.bindValue(5, "--");
        query.bindValue(6, "--");
        query.exec();
    }

    query.prepare("insert into aip_record values(?,?,?,?,?,?,?,?,?)");
    query.bindValue(0, recUuid);
    query.bindValue(1, devUuid);
    query.bindValue(2, devAddr);
    query.bindValue(3, devNumb);
    query.bindValue(4, netPort);
    query.bindValue(5, netAddr);
    query.bindValue(6, recTime);
    query.bindValue(7, netStat);
    query.bindValue(8, verNumb);
    if (!query.exec()) {
        qDebug() << query.lastError();
    }

    query.prepare("insert into aip_online values(?,?,?,?,?,?,?,?,?)");
    query.bindValue(0, recUuid);
    query.bindValue(1, devUuid);
    query.bindValue(2, devAddr);
    query.bindValue(3, devNumb);
    query.bindValue(4, netPort);
    query.bindValue(5, netAddr);
    query.bindValue(6, recTime);
    query.bindValue(7, netStat);
    query.bindValue(8, verNumb);
    if (!query.exec()) {
        qDebug() << query.lastError();
    }
}

void SqlServer::deleteOnline(QJsonObject msg)
{
    double recUuid = snow.getId();
    double devUuid = snow.getId();
    QString devAddr = msg.value("devAddr").toString();
    QString devNumb = msg.value("devNumb").toString();
    QString netPort = QString::number(msg.value("netPort").toInt());
    QString netAddr = msg.value("netAddr").toString();
    QString recTime = msg.value("recTime").toString();
    QString netStat = msg.value("netStat").toString();
    QString verNumb = msg.value("verNumb").toString();

    QSqlQuery query(db);

    query.prepare("select devUuid from aip_device where devAddr=:devAddr");
    query.bindValue(":devAddr", devAddr);
    if (!query.exec()) {
        qDebug() << query.lastError();
        return;
    }
    if (!query.next()) {
        qDebug() << "no such device";
        return;
    }

    devUuid = query.value(0).toDouble();
    query.prepare("delete from aip_online where devAddr=:devAddr and devNumb=:devNumb");
    query.bindValue(":devAddr", devAddr);
    query.bindValue(":devNumb", devNumb);
    if (!query.exec()) {
        qDebug() << query.lastError();
    }

    query.prepare("insert into aip_record values(?,?,?,?,?,?,?,?,?)");
    query.bindValue(0, recUuid);
    query.bindValue(1, devUuid);
    query.bindValue(2, devAddr);
    query.bindValue(3, devNumb);
    query.bindValue(4, netPort);
    query.bindValue(5, netAddr);
    query.bindValue(6, recTime);
    query.bindValue(7, netStat);
    query.bindValue(8, verNumb);
    if (!query.exec()) {
        qDebug() << query.lastError();
    }
}

bool SqlServer::checkMaster(QJsonObject msg)
{
    QSqlQuery query(db);
    query.prepare("select id from aip_master where name=? and password=?");
    query.addBindValue(msg.value("username").toString());
    query.addBindValue(msg.value("password").toString());
    if (!query.exec()) {
        qDebug() << query.lastError();
        return false;
    }
    if (query.next()) {
        return true;
    } else {
        return false;
    }
}


