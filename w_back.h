#ifndef W_BACK_H
#define W_BACK_H

#include <QWidget>
#include <QTimer>
#include <QLayout>
#include <QStackedWidget>
#include <my_define.h>

#include <w_modl.h>
#include <myhelper.h>
#include <tcpclient.h>
#include <tcpserver.h>
class w_Back : public QWidget
{
    Q_OBJECT
    
public:
    w_Back(QWidget *parent = 0);
    ~w_Back();

private:
    int sysSetp;
    QStackedWidget *stack;

    w_Modl *wModl;

    QStringList data;
    QTcpSocket *tcpClient;
    tcpServer *server;
private slots:
    void Init();
    //客户端槽函数
    void ReadData();
    void ReadError(QAbstractSocket::SocketError);

    //服务端槽函数
    void ClientReadData(int clientID,QString IP,int Port,QByteArray data);
    void ClientConnect(int clientID,QString IP,int Port);
    void ClientDisConnect(int clientID,QString IP,int Port);
public slots:
    void system();
};

#endif // W_BACK_H
