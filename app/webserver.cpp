/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       服务器
*******************************************************************************/
#include "webserver.h"

WebServer::WebServer(QObject *parent) : QObject(parent)
{
    initSql();
}

void WebServer::initServer()
{
    tcp = new TcpServer(this);
    connect(tcp, SIGNAL(newConnection(quint16)), this, SLOT(newClient(quint16)));
    if (!tcp->listen(QHostAddress::AnyIPv4, 6000))
        qDebug() << "listen 6000 fail";

    usr = new TcpServer(this);
    connect(usr, SIGNAL(newConnection(quint16)), this, SLOT(newMaster(quint16)));
    if (!usr->listen(QHostAddress::AnyIPv4, 5999))
        qDebug() << "listen 5999 fail";

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(check()));
    timer->start(5000);
}

void WebServer::newClient(quint16 qrt)
{
    TcpSocket *s = new TcpSocket;
    s->setSocketDescriptor(qrt);
    connect(s, SIGNAL(disconnected()), this, SLOT(disClient()));
    connect(s, SIGNAL(signin(QByteArray)), this, SLOT(newOnline(QByteArray)));
    connect(s, SIGNAL(display(QString)), this, SLOT(display(QString)));
    connect(s, SIGNAL(command(quint16, quint16, QByteArray)),
            this, SLOT(recvClient(quint16, quint16, QByteArray)));
    clients[qrt] = s;
    objs[qrt].insert("netAddr", s->peerAddress().toString());
    objs[qrt].insert("netPort", s->peerPort());
    objs[qrt].insert("recTime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    qDebug() << "new client" << s->peerPort();
}

void WebServer::newMaster(quint16 qrt)
{
    TcpSocket *s = new TcpSocket;
    s->setSocketDescriptor(qrt);
    masters[qrt] = s;
    connect(s, SIGNAL(disconnected()), this, SLOT(disMaster()));
    connect(s, SIGNAL(signin(QByteArray)), this, SLOT(newSignin(QByteArray)));
    connect(s, SIGNAL(command(quint16, quint16, QByteArray)),
            this, SLOT(recvMaster(quint16, quint16, QByteArray)));
    qDebug() << "new master" << s->peerPort();
}

void WebServer::newOnline(QByteArray msg)
{
    QStringList dev = QString(msg).split(" ");
    if (dev.size() >= 3) {
        TcpSocket *s = reinterpret_cast<TcpSocket*>(sender());
        int qrt = s->socketDescriptor();
        objs[qrt].insert("devNumb", dev.at(0));
        objs[qrt].insert("devAddr", dev.at(1));
        objs[qrt].insert("verNumb", dev.at(2));
        objs[qrt].insert("netStat", tr("上线"));
        sql->appendOnline(objs[qrt]);
    }
}

void WebServer::newSignin(QByteArray msg)
{
    QJsonObject obj = QJsonDocument::fromJson(QByteArray::fromBase64(msg)).object();
    if (sql->checkMaster(obj)) {
        QJsonObject mysql;
        mysql.insert("hostname", "192.168.1.55");
        mysql.insert("username", "root");
        mysql.insert("password", "87973318");
        mysql.insert("database", "aip-server");
        TcpSocket *s = reinterpret_cast<TcpSocket*>(sender());
        QByteArray sign = QJsonDocument(mysql).toJson();
        s->sendSocket(ADDR, SIGN_OK, sign.toBase64());
        qDebug() << "new sign in" << obj;
    } else {
        qDebug() << "new sign in error" << obj;
    }
}

void WebServer::disClient()
{
    TcpSocket *s = reinterpret_cast<TcpSocket*>(sender());
    qDebug() << "client disconnect" << s->peerPort();
    foreach(int i, clients.keys()) {
        if (clients[i]->peerPort() == s->peerPort()) {
            objs[i].insert("netStat", tr("下线"));
            sql->deleteOnline(objs[i]);
            clients[i]->deleteLater();
            clients.remove(i);
            objs.remove(i);
            break;
        }
    }
}

void WebServer::disMaster()
{
    TcpSocket *s = reinterpret_cast<TcpSocket*>(sender());
    qDebug() << "master disconnect" << s->peerPort();
    foreach(int i, masters.keys()) {
        if (masters[i]->peerPort() == s->peerPort()) {
            masters[i]->deleteLater();
            masters.remove(i);
            break;
        }
    }
}

void WebServer::display(QString msg)
{
    TcpSocket *s = reinterpret_cast<TcpSocket*>(sender());
    int qrt = s->socketDescriptor();
    if (objs[qrt].keys().contains("connect")) {
        int handle = objs[qrt].value("connect").toInt();
        qDebug() << "recv display" << msg;
        if (masters.keys().contains(handle)) {
            masters[handle]->sendSocket(ADDR, SHOW, msg.toUtf8());
        }
    }
}

void WebServer::recvClient(quint16 addr, quint16 cmd, QByteArray msg)
{
    TcpSocket *s = reinterpret_cast<TcpSocket*>(sender());
    int qrt = s->socketDescriptor();
    if (objs[qrt].keys().contains("connect")) {
        int handle = objs[qrt].value("connect").toInt();
        qDebug() << "client" << addr << cmd << msg;
        if (masters.keys().contains(handle)) {
            masters[handle]->sendSocket(addr, cmd, msg);
        }
    }
}

void WebServer::recvMaster(quint16 addr, quint16 cmd, QByteArray msg)
{
    TcpSocket *s = reinterpret_cast<TcpSocket*>(sender());
    foreach(int i, clients.keys()) {
        if (clients[i]->peerPort() == addr) {
            qDebug() << "master" << addr << cmd << msg;
            switch (cmd) {
            case SEND_HEAD:
                clients[i]->sendFileHead(msg);
                break;
            default:
                clients[i]->sendSocket(ADDR, cmd, msg);
                break;
            }
            objs[i].insert("connect", s->socketDescriptor());
            break;
        }
    }
}

void WebServer::initSql()
{
    sql = new Sqlite(this);
    sql->connect();
}

void WebServer::check()
{
    foreach(int i, clients.keys()) {
        clients[i]->heart++;
        if (clients[i]->heart > 5)
            clients[i]->abort();
    }
    foreach(int i, masters.keys()) {
        masters[i]->heart++;
        if (masters[i]->heart > 3)
            masters[i]->abort();
    }
}

