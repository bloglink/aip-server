#include "SqlServer.h"

SqlServer::SqlServer(QObject *parent) : QObject(parent)
{

}

/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2015.11.21
  * brief:      打开数据库
******************************************************************************/
void SqlServer::Init()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(SQL);
    db.open();
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2015.11.21
  * brief:      退出
******************************************************************************/
void SqlServer::Quit(void)
{
    QString name;
    {
        name = QSqlDatabase::database().connectionName();
    }//超出作用域，隐含对象QSqlDatabase::database()被删除。
    QSqlDatabase::removeDatabase(name);
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2015.11.21
  * brief:      添加状态记录
******************************************************************************/
void SqlServer::InsertState(QStringList msg)
{
    if (msg.size() != 6)
        return;
    QSqlQuery query(db);

    query.exec("select max(id) from NetRecord");
    query.next();
    int max = query.value(0).toInt();

    QString t = QDateTime::currentDateTime().toString("yyyyMMddhmss");

    QString in = "insert into NetRecord (ID,IP,NO,MAC,TIME,STATE,VERSION)";
    in += "values(:ID,:IP,:NO,:MAC,:TIME,:STATE,:VERSION)";
    query.prepare(in);
    query.bindValue(":ID",     max+1);
    query.bindValue(":IP",     msg.at(1));
    query.bindValue(":NO",     msg.at(3));
    query.bindValue(":MAC",    msg.at(4));
    query.bindValue(":TIME",   t);
    query.bindValue(":STATE",  msg.at(0));
    query.bindValue(":VERSION",msg.at(5));
    query.exec();
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2015.11.21
  * brief:      添加信任记录
******************************************************************************/
void SqlServer::InsertTrust(QString msg)
{
    QSqlQuery query(db);

    query.exec("select max(id) from NetTrust");
    query.next();
    int max = query.value(0).toInt();

    QString in = "insert into NetTrust (ID,MAC)";
    in += "values(:ID,:MAC)";
    query.prepare(in);
    query.bindValue(":ID", max+1);
    query.bindValue(":MAC",msg);
    query.exec();
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2015.11.21
  * brief:      添加
******************************************************************************/
void SqlServer::InsertUsers(QString msg)
{
    QSqlQuery query(db);

    query.exec("select max(id) from NetUsers");
    query.next();
    int max = query.value(0).toInt();

    QString in = "insert into NetUsers (ID,USER)";
    in += "values(:ID,:USER)";
    query.prepare(in);
    query.bindValue(":ID", max+1);
    query.bindValue(":USER",msg);
    query.exec();
}

QStringList SqlServer::ListUsers()
{
    QSqlQuery query(db);
    QStringList temp;

    query.exec("select USER from NetUsers");
    while(query.next())
        temp.append(query.value(0).toString());
    return temp;
}

QStringList SqlServer::ListTrust()
{
    QSqlQuery query(db);
    QStringList temp;

    query.exec("select MAC from NetTrust");
    while(query.next())
        temp.append(query.value(0).toString());
    return temp;
}
