#include "tcpclient.h"

tcpClient::tcpClient(QObject *parent,int clientID) :
    QTcpSocket(parent)
{
    this->clientID = clientID;
    Info.id = QString::number(clientID);

    connect(this,SIGNAL(readyRead()),this,SLOT(ReadData()));//挂接读取数据信号
    connect(this,SIGNAL(disconnected()),this,SLOT(DisConnect()));//关闭连接时，发送断开连接信号
    //如果关闭连接自动删除，则下次不能再次监听，奇怪的问题
    connect(this,SIGNAL(disconnected()),this,SLOT(deleteLater()));//关闭连接时，对象自动删除
}
void tcpClient::ReadData()
{
    QTime dieTime = QTime::currentTime().addMSecs(100);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

    //读取完整一条数据并发送信号
    QByteArray data=this->readAll();
    Info.ip = this->peerAddress().toString();
    Info.port = QString::number(this->peerPort());
    emit ClientReadData(this->clientID,this->peerAddress().toString(),this->peerPort(),data);
}

void tcpClient::DisConnect()
{
    //断开连接时，发送断开信号
    emit ClientDisConnect(this->clientID,this->peerAddress().toString(),this->peerPort());
}
