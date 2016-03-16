#include "w_back.h"

w_Back::w_Back(QWidget *parent)
    : QWidget(parent)
{
    wModl = NULL;

    stack = new QStackedWidget(this);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(stack);
    this->setLayout(layout);
    layout->setContentsMargins(0,0,0,0);

    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(system()));
    timer->start(50);
    sysSetp = 0;
}

w_Back::~w_Back()
{
    
}

void w_Back::Init()
{
    tcpClient=new QTcpSocket(this);
    tcpClient->abort();//取消原有连接
    connect(tcpClient,SIGNAL(readyRead()),this,SLOT(ReadData()));
    connect(tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(ReadError(QAbstractSocket::SocketError)));

    server = new tcpServer(this);

    connect(server,SIGNAL(ClientConnect(int,QString,int)),
            this,SLOT(ClientConnect(int,QString,int)));
    connect(server,SIGNAL(ClientDisConnect(int,QString,int)),
            this,SLOT(ClientDisConnect(int,QString,int)));
    connect(server,SIGNAL(ClientReadData(int,QString,int,QByteArray)),
            this,SLOT(ClientReadData(int,QString,int,QByteArray)));
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2015.12.10
  * brief:      系统后台功能
******************************************************************************/
void w_Back::system()
{
    //初始化界面
    switch (sysSetp) {
    case 0:
        wModl = new w_Modl(this);
        stack->insertWidget(stack->count(),wModl);
        stack->setCurrentIndex(0);
        break;
    case 1:
        Init();
        server->listen(QHostAddress::Any,6000);
        break;
    default:
        break;

    }
    sysSetp++;
}


void w_Back::ReadData()
{
    QByteArray buffer=tcpClient->readAll();
    if (!buffer.isEmpty())
    {

    }
}

void w_Back::ReadError(QAbstractSocket::SocketError)
{
    tcpClient->disconnectFromHost();
}
void w_Back::ClientReadData(int clientID,QString IP,int Port,QByteArray data)
{
    if (!data.isEmpty())
    {
        qDebug()<<clientID;
        qDebug()<<IP;
        qDebug()<<Port;
    }
}

void w_Back::ClientConnect(int clientID,QString IP,int Port)
{
    wModl->id.append(QString::number(clientID));
    wModl->ip.append(IP);
    wModl->port.append(QString::number(Port));
    wModl->time.append(QTime::currentTime().toString());
    wModl->status.append(tr("上线"));
    wModl->updateShow();
}

void w_Back::ClientDisConnect(int clientID,QString IP,int Port)
{
    int index = wModl->id.indexOf(QString::number(clientID));

    wModl->id.removeAt(index);
    wModl->ip.removeAt(index);
    wModl->port.removeAt(index);
    wModl->time.append(QTime::currentTime().toString());
}
