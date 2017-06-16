#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <QDebug>
#include <QObject>
#include <QThread>
#include <QJsonObject>

#include "TcpServer.h"
#include "TcpSocket.h"
#include "SqlServer.h"

class WebServer : public QObject
{
    Q_OBJECT
public:
    explicit WebServer(QObject *parent = 0);

signals:
    void SendJson(QJsonObject json);
private slots:
    void Init(void);
    void InitSocket(quint16 id);
    void ReadJson(QJsonObject json);
    void Logout(QJsonObject json);
    void PutOnline(QJsonObject json);
private:
    QList<QThread*> threads;
    SqlServer *sql;
    QList<QJsonObject> lists;

};

#endif // WEBSERVER_H
