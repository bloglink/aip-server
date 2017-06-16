#include "SqlServer.h"

SqlServer::SqlServer(QObject *parent) : QObject(parent)
{
    QDir *dir = new QDir;
    if (!dir->exists(QString(SQL_PATH)))
        dir->mkdir(QString(SQL_PATH));
}

void SqlServer::Init()
{
    QFile file(SQL);
    if (!file.exists()) {
        file.open(QIODevice::ReadWrite);
        file.close();
    }
    db = QSqlDatabase::addDatabase("QSQLITE","Server");
    db.setDatabaseName(SQL);
    db.open();

    QSqlQuery query(db);
    query.exec("select count(*) from sqlite_master where type='table' and name='Log'");
    if(query.next() && query.value(0).toInt() == 0) {
        query.prepare("create table Log (ID text primary key,IP text,NO text,MAC text,TIME text,STATE text,VERSION text)");
        query.exec();
    }
}

void SqlServer::Insert(QJsonObject json)
{
    QSqlQuery query(db);
    QString in = "insert into Log (ID,IP,NO,MAC,TIME,STATE,VERSION)";
    in += "values(:ID,:IP,:NO,:MAC,:TIME,:STATE,:VERSION)";
    query.prepare(in);
    query.bindValue(":ID",     QUuid::createUuid().toString());
    query.bindValue(":IP",     json.value("TxAddr").toString());
    query.bindValue(":NO",     json.value("TxUser").toString());
    query.bindValue(":MAC",    json.value("TxAddrMac").toString());
    query.bindValue(":TIME",   json.value("TxTime").toString());
    query.bindValue(":STATE",  json.value("TxMessage").toString());
    query.bindValue(":VERSION",json.value("TxVersion").toString());
    query.exec();
}

//void SqlServer::GuestRemove(TcpMap map, QByteArray msg)
//{
//    QSqlQuery query(db);

//    QString Number   = QString(msg).split(" ").at(0);
//    QString HardAddr = QString(msg).split(" ").at(1);

//    if (Number.contains("@@")) {
//        Number = Number.split("@@").at(0);
//    }
//    query.prepare("select ID from NetState where NO=:NO and MAC=:MAC");
//    query.bindValue(":NO",Number);
//    query.bindValue(":MAC",HardAddr);
//    query.exec();
//    if (query.next()) {
//        int index = query.value(0).toInt();
//        query.prepare("delete from NetState where ID=:ID");
//        query.bindValue(":ID",index);
//        query.exec();
//    }
//}

//void SqlServer::GuestLogin(TcpMap map, QByteArray msg)
//{
//    QString UserName = QString(msg).split(" ").at(0);
//    QString HardAddr = QString(msg).split(" ").at(1);
//    QString Password = NULL;
//    quint16 TxAddress = map.value("TxAddress");
//    quint16 RxAddress = map.value("RxAddress");
//    map["TxAddress"] = RxAddress;
//    map["RxAddress"] = TxAddress;

//    if (UserName.contains("@@")) {
//        Password = UserName.split("@@").at(1);
//        UserName = UserName.split("@@").at(0);
//    }
//    QSqlQuery query(db);

//    query.prepare("select USER,PASSWORD from NetUsers where MAC = :mac");
//    query.bindValue(":mac",HardAddr);
//    query.exec();
//    if (!query.next()) {
//        map["TxCommand"] = ADMIN_LOGIN_ERROR;
//        msg = "未信任的主机,请联系管理员添加信任";
//        emit SendMessage(map,msg);
//        return;
//    }
//    QString user1 = UserName;
//    QString user2 = query.value(0).toString();
//    QString pass1 = Password;
//    QString pass2 = query.value(1).toString();
//    if (user1 != user2 || pass1 != pass2) {
//        map["TxCommand"] = ADMIN_LOGIN_ERROR;
//        msg = "用户或密码错误";
//        emit SendMessage(map,msg);
//        return;
//    }
//    map["TxCommand"] = ADMIN_LOGIN_SUCCESS;
//    msg = "验证成功";
//    emit SendMessage(map,msg);
//}
//void SqlServer::GuestStatus(TcpMap map, QByteArray msg)
//{
//    QSqlQuery query(db);

//    QString t = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");

//    quint16 TxCommand = map.value("TxCommand");
//    quint16 RxAddress = map.value("RxAddress");
//    QString Number   = QString(msg).split(" ").at(0);
//    QString HardAddr = QString(msg).split(" ").at(1);
//    QString Version  = QString(msg).split(" ").at(2);
//    QString Address = QString(msg).split(" ").at(3);
//    QString State    = NULL;
//    if (Number.contains("@@")) {
//        Number = Number.split("@@").at(0);
//    }
//    switch (TxCommand) {
//    case ADMIN_LOGIN:
//    case GUEST_LOGIN:
//        State = "上线";
//        break;
//    case GUEST_DROPED:
//        State = "下线";
//        break;
//    default:
//        break;
//    }

//    query.exec("select max(id) from NetState");
//    query.next();
//    int max = query.value(0).toInt();

//    QString up = "insert into NetState (ID,PORT,IP,NO,MAC,TIME,STATE,VERSION)";
//    up += "values(:ID,:PORT,:IP,:NO,:MAC,:TIME,:STATE,:VERSION)";
//    query.prepare(up);
//    query.bindValue(":ID",     max+1);
//    query.bindValue(":PORT",   RxAddress);
//    query.bindValue(":IP",     Address);
//    query.bindValue(":NO",     Number);
//    query.bindValue(":MAC",    HardAddr);
//    query.bindValue(":TIME",   t);
//    query.bindValue(":STATE",  State);
//    query.bindValue(":VERSION",Version);
//    query.exec();
//}



