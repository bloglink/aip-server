#ifndef W_HOME_H
#define W_HOME_H

#include <QWidget>
#include <tcpclient.h>
#include <tcpserver.h>
#include <QByteArray>
#include <QTimer>

namespace Ui {
class w_Home;
}

class w_Home : public QWidget
{
    Q_OBJECT
    
public:
    explicit w_Home(QWidget *parent = 0);
    ~w_Home();
    

private:
    Ui::w_Home *ui;

private:
    bool isServerOn;
    tcpServer *server;

private slots:
    void ClientReadData(int clientID,QString IP,int Port,QByteArray data);
    void ClientConnect(int clientID,QString IP,int Port);
    void ClientDisConnect(int clientID,QString IP,int Port);
    void updateShow();
    void heartBeat();

    void on_pushButton_clicked();

    void on_pushButtonStart_clicked();
public:
    QStringList id;
    QStringList ip;
    QStringList port;
    QStringList time;
    QStringList status;
    QList<int> heartCount;

};

#endif // W_HOME_H
