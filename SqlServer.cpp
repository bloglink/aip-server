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
    db = QSqlDatabase::addDatabase("QSQLITE","Server");
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
void SqlServer::InsertState(QUrl url)
{
    QSqlQuery query(db);

    query.exec("select max(id) from NetRecord");
    query.next();
    int max = query.value(0).toInt();

    QString t = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");

    QString Number   = url.fragment().split(" ").at(0);
    QString HardAddr = url.fragment().split(" ").at(1);
    QString Version  = url.fragment().split(" ").at(2);
    QString State    = NULL;
    quint16 fun = url.query().toInt();
    if (Number.contains("@@")) {
        Number = Number.split("@@").at(0);
    }
    switch (fun) {
    case ADMIN_LOGIN:
        UsersCheck(url);
    case GUEST_LOGIN:
        State = "上线";
        break;
    case GUEST_DROPED:
        State = "下线";
        break;
    default:
        break;
    }
    QString in = "insert into NetRecord (ID,IP,NO,MAC,TIME,STATE,VERSION)";
    in += "values(:ID,:IP,:NO,:MAC,:TIME,:STATE,:VERSION)";
    query.prepare(in);
    query.bindValue(":ID",     max+1);
    query.bindValue(":IP",     url.host());
    query.bindValue(":NO",     Number);
    query.bindValue(":MAC",    HardAddr);
    query.bindValue(":TIME",   t);
    query.bindValue(":STATE",  State);
    query.bindValue(":VERSION",Version);
    query.exec();

    query.prepare("select ID from NetState where NO=:NO and MAC=:MAC");
    query.bindValue(":NO",Number);
    query.bindValue(":MAC",HardAddr);
    query.exec();
    if (query.next()) {
        max = query.value(0).toInt();
        query.prepare("delete from NetState where ID=:ID");
        query.bindValue(":ID",max);
        query.exec();

    }
    query.exec("select max(id) from NetState");
    query.next();
    max = query.value(0).toInt();

    QString up = "insert into NetState (ID,PORT,IP,NO,MAC,TIME,STATE,VERSION)";
    up += "values(:ID,:PORT,:IP,:NO,:MAC,:TIME,:STATE,:VERSION)";
    query.prepare(up);
    query.bindValue(":ID",     max+1);
    query.bindValue(":PORT",   url.userName());
    query.bindValue(":IP",     url.host());
    query.bindValue(":NO",     Number);
    query.bindValue(":MAC",    HardAddr);
    query.bindValue(":TIME",   t);
    query.bindValue(":STATE",  State);
    query.bindValue(":VERSION",Version);
    query.exec();
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2015.11.21
  * brief:      验证用户名密码
******************************************************************************/
void SqlServer::UsersCheck(QUrl url)
{
    QSqlQuery query(db);

    QString UserName = url.fragment().split(" ").at(0);
    QString HardAddr = url.fragment().split(" ").at(1);

    if (UserName.contains("@@")) {
        url.setPassword(UserName.split("@@").at(1));
        UserName = UserName.split("@@").at(0);
    }

    url.setPort(url.userName().toInt());
    url.setUserName(QString::number(ADDR));

    query.prepare("select USER,PASSWORD from NetUsers where MAC = :mac");
    query.bindValue(":mac",HardAddr);
    query.exec();
    if (!query.next()) {
        url.setQuery(QString::number(ADMIN_LOGIN_ERROR));
        url.setFragment("未信任的主机,请联系管理员添加信任");
        emit TransformCmd(url);
        return;
    }
    QString user1 = UserName;
    QString user2 = query.value(0).toString();
    QString pass1 = url.password();
    QString pass2 = query.value(1).toString();
    if (user1 != user2 || pass1 != pass2) {
        url.setQuery(QString::number(ADMIN_LOGIN_ERROR));
        url.setFragment("用户或密码错误");
        emit TransformCmd(url);
        return;
    }
    url.setQuery(QString::number(ADMIN_LOGIN_SUCCESS));
    url.setFragment("验证成功");
    emit TransformCmd(url);
}

