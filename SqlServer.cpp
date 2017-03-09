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
void SqlServer::ReadMessage(TcpMap map, QByteArray msg)
{
    if (msg.isEmpty())
        return;
    quint16 TxCommand = map.value("TxCommand");
    switch (TxCommand) {
    case ADMIN_LOGIN:
        GuestLogin(map,msg);
    case GUEST_LOGIN:
    case GUEST_DROPED:
        GuestRecord(map,msg);
        GuestRemove(map,msg);
        GuestStatus(map,msg);
        break;
    default:
        break;
    }
}

void SqlServer::GuestRecord(TcpMap map, QByteArray msg)
{
    QSqlQuery query(db);

    query.exec("select max(id) from NetRecord");
    query.next();
    int max = query.value(0).toInt();

    QString t = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");

    quint16 TxCommand = map.value("TxCommand");
    QString Number   = QString(msg).split(" ").at(0);
    QString HardAddr = QString(msg).split(" ").at(1);
    QString Version  = QString(msg).split(" ").at(2);
    QString Address = QString(msg).split(" ").at(3);
    QString State    = NULL;

    if (Number.contains("@@")) {
        Number = Number.split("@@").at(0);
    }
    switch (TxCommand) {
    case ADMIN_LOGIN:
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
    query.bindValue(":IP",     Address);
    query.bindValue(":NO",     Number);
    query.bindValue(":MAC",    HardAddr);
    query.bindValue(":TIME",   t);
    query.bindValue(":STATE",  State);
    query.bindValue(":VERSION",Version);
    query.exec();
}
void SqlServer::GuestRemove(TcpMap map, QByteArray msg)
{
    QSqlQuery query(db);

    QString Number   = QString(msg).split(" ").at(0);
    QString HardAddr = QString(msg).split(" ").at(1);

    if (Number.contains("@@")) {
        Number = Number.split("@@").at(0);
    }
    query.prepare("select ID from NetState where NO=:NO and MAC=:MAC");
    query.bindValue(":NO",Number);
    query.bindValue(":MAC",HardAddr);
    query.exec();
    if (query.next()) {
        int index = query.value(0).toInt();
        query.prepare("delete from NetState where ID=:ID");
        query.bindValue(":ID",index);
        query.exec();
    }
}

void SqlServer::GuestLogin(TcpMap map, QByteArray msg)
{
    QString UserName = QString(msg).split(" ").at(0);
    QString HardAddr = QString(msg).split(" ").at(1);
    QString Password = NULL;
    quint16 TxAddress = map.value("TxAddress");
    quint16 RxAddress = map.value("RxAddress");
    map["TxAddress"] = RxAddress;
    map["RxAddress"] = TxAddress;

    if (UserName.contains("@@")) {
        Password = UserName.split("@@").at(1);
        UserName = UserName.split("@@").at(0);
    }
    QSqlQuery query(db);

    query.prepare("select USER,PASSWORD from NetUsers where MAC = :mac");
    query.bindValue(":mac",HardAddr);
    query.exec();
    if (!query.next()) {
        map["TxCommand"] = ADMIN_LOGIN_ERROR;
        msg = "未信任的主机,请联系管理员添加信任";
        emit SendMessage(map,msg);
        return;
    }
    QString user1 = UserName;
    QString user2 = query.value(0).toString();
    QString pass1 = Password;
    QString pass2 = query.value(1).toString();
    if (user1 != user2 || pass1 != pass2) {
        map["TxCommand"] = ADMIN_LOGIN_ERROR;
        msg = "用户或密码错误";
        emit SendMessage(map,msg);
        return;
    }
    map["TxCommand"] = ADMIN_LOGIN_SUCCESS;
    msg = "验证成功";
    emit SendMessage(map,msg);
}
void SqlServer::GuestStatus(TcpMap map, QByteArray msg)
{
    QSqlQuery query(db);

    QString t = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");

    quint16 TxCommand = map.value("TxCommand");
    quint16 RxAddress = map.value("RxAddress");
    QString Number   = QString(msg).split(" ").at(0);
    QString HardAddr = QString(msg).split(" ").at(1);
    QString Version  = QString(msg).split(" ").at(2);
    QString Address = QString(msg).split(" ").at(3);
    QString State    = NULL;
    if (Number.contains("@@")) {
        Number = Number.split("@@").at(0);
    }
    switch (TxCommand) {
    case ADMIN_LOGIN:
    case GUEST_LOGIN:
        State = "上线";
        break;
    case GUEST_DROPED:
        State = "下线";
        break;
    default:
        break;
    }

    query.exec("select max(id) from NetState");
    query.next();
    int max = query.value(0).toInt();

    QString up = "insert into NetState (ID,PORT,IP,NO,MAC,TIME,STATE,VERSION)";
    up += "values(:ID,:PORT,:IP,:NO,:MAC,:TIME,:STATE,:VERSION)";
    query.prepare(up);
    query.bindValue(":ID",     max+1);
    query.bindValue(":PORT",   RxAddress);
    query.bindValue(":IP",     Address);
    query.bindValue(":NO",     Number);
    query.bindValue(":MAC",    HardAddr);
    query.bindValue(":TIME",   t);
    query.bindValue(":STATE",  State);
    query.bindValue(":VERSION",Version);
    query.exec();
}



