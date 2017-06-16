#include "WebServer.h"

WebServer::WebServer(QObject *parent) : QObject(parent)
{
    Init();
}

void WebServer::Init()
{
    for (int i=0; i<QThread::idealThreadCount(); i++) {
        threads.append(new QThread(this));
        threads.at(i)->start();
    }

    sql = new SqlServer(this);
    sql->Init();

    TcpServer *s = new TcpServer(this);
    connect(s,SIGNAL(InitSocket(quint16)),this,SLOT(InitSocket(quint16)));
    s->listen(QHostAddress::AnyIPv4,5999);
}

void WebServer::InitSocket(quint16 handle)
{
    TcpSocket *s = new TcpSocket;
    s->setSocketDescriptor(handle);
    s->moveToThread(threads.at(handle%threads.size()));
    connect(s,SIGNAL(SendJson(QJsonObject)),this,SLOT(ReadJson(QJsonObject)));
    connect(this,SIGNAL(SendJson(QJsonObject)),s,SLOT(ReadJson(QJsonObject)));
    qDebug()<<QTime::currentTime().toString()<<s->peerAddress().toString();
}

void WebServer::ReadJson(QJsonObject json)
{
    if (json.value("TxAddress").toInt() != ADDR) {
        emit SendJson(json);
        return;
    }
    quint16 TxCommand = json.value("TxCommand").toInt();
    switch (TxCommand) {
    case GUEST_LOGIN:
        sql->Insert(json);
        lists.append(json);
        break;
    case GUEST_DROPED:
        sql->Insert(json);
        Logout(json);
        break;
    case ONLINE_DEVICES:
        PutOnline(json);
        break;
    default:
        break;
    }
}

void WebServer::Logout(QJsonObject json)
{
    int p = json.value("RxAddress").toInt();
    int t = lists.size();
    for (int i=0; i<t; i++) {
        if (lists.at(i).value("RxAddress").toInt() == p) {
            lists.removeAt(i);
            break;
        }
    }
}

void WebServer::PutOnline(QJsonObject json)
{
    quint16 TxAddress = json.value("TxAddress").toInt();
    quint16 RxAddress = json.value("RxAddress").toInt();
    QStringList ItemText;
    for (int i=0; i<lists.size(); i++) {
        QStringList s;
        s.append(QString::number(lists.at(i).value("RxAddress").toInt()));
        s.append(lists.at(i).value("TxAddr").toString());
        s.append(lists.at(i).value("TxTime").toString());
        s.append(lists.at(i).value("TxUser").toString());
        s.append(lists.at(i).value("TxAddrMac").toString());
        s.append(lists.at(i).value("TxVersion").toString());
        ItemText.append(s.join("@@"));
    }
    json.insert("TxAddress",RxAddress);
    json.insert("RxAddress",TxAddress);
    json.insert("TxMessage",ItemText.join("\n"));
    emit SendJson(json);
}

