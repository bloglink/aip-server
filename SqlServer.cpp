#include "SqlServer.h"

SqlServer::SqlServer(QObject *parent) : QObject(parent)
{

}

void SqlServer::SqlOpen(QString name)
{
    db = QSqlDatabase::addDatabase("QSQLITE",name);
    db.setDatabaseName(SQL);
    db.open();
}

void SqlServer::SqlClose(void)
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
  * brief:      插入一条记录
******************************************************************************/
void SqlServer::SqlInsert(QString msg)
{
    QSqlQuery query(db);
    QStringList p = msg.split(" ");

    if (p.size() != 6)
        return;

    SqlCreate(p.at(3));
    int max = SqlMaxCount(p.at(3));

    QString in = "insert into :table (ID,IP,NO,MAC,TIME,STATE,VERSION)";
    in += "values(:ID,:IP,:NO,:MAC,:TIME,:STATE,:VERSION)";
    in.replace(":table",p.at(3));

    query.prepare(in);
    query.bindValue(":ID",max+1);
    query.bindValue(":IP",p.at(1));
    query.bindValue(":NO",p.at(3));
    query.bindValue(":MAC",p.at(4));
    query.bindValue(":TIME",QTime::currentTime().toString());
    query.bindValue(":STATE",p.at(6));
    query.bindValue(":VERSION",p.at(5));
    query.exec();
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2015.11.21
  * brief:      创建表
******************************************************************************/
void SqlServer::SqlCreate(QString msg)
{
    if (SqlIsExist(msg))
        return;

    QString in = "create table :table (ID int primary key,";
    in += "IP varchar(20),";
    in += "NO varchar(20),";
    in += "MAC varchar(20),";
    in += "TIME varchar(20),";
    in += "STATE varchar(20),";
    in += "VERSION varchar(20))";
    in.replace(":table",msg);

    QSqlQuery query(db);
    query.prepare(in);
    query.exec();
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2015.11.21
  * brief:      判断表是否存在
******************************************************************************/
bool SqlServer::SqlIsExist(QString msg)
{
    QSqlQuery query(db);

    query.prepare("select count(*) from sqlite_master where type='table' and name=:name");
    query.bindValue(":name",msg);
    query.exec( );
    if(query.next())
    {
        if(query.value(0).toInt() == 0)
            return false;
        else
            return true;
    }
    return false;
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2015.11.21
  * brief:      获取最大ID
******************************************************************************/
int SqlServer::SqlMaxCount(QString msg)
{
    QSqlQuery query(db);
    QString m = QString("select max(id) from %1").arg(msg);
    query.exec(m);
    query.next();
    return query.value(0).toInt();
}
