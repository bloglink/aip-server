#include "tcpclient.h"

tcpClient::tcpClient(QObject *parent,int clientID) :
    QTcpSocket(parent)
{
    this->clientID = clientID;
    this->clientHeart = 0;
    Info.Id = QString::number(clientID);


    bytesReceived = 0;

    connect(this,SIGNAL(readyRead()),this,SLOT(ReadData()));//挂接读取数据信号
    connect(this,SIGNAL(disconnected()),this,SLOT(DisConnect()));//关闭连接时，发送断开连接信号
    //如果关闭连接自动删除，则下次不能再次监听，奇怪的问题
//    connect(this,SIGNAL(disconnected()),this,SLOT(deleteLater()));//关闭连接时，对象自动删除
}
void tcpClient::ReadData()
{
    this->clientHeart = 0;
    Info.Port = QString::number(this->peerPort());

    QByteArray data = this->readAll();

    quint8 fun = quint8(data[0]);
    QByteArray tt;

    switch (fun) {
    case sendtype_heart:
        break;
    case sendtype_msg:
        data.remove(0,1);
        Info.Ip = this->peerAddress().toString();
        Info.Port = QString::number(this->peerPort());
        emit ClientReadData(this->clientID,this->peerAddress().toString(),
                            this->peerPort(),data);
        break;
    case type_ip:
        data.remove(0,1);
        Info.Ip = data;
        tt[0] = type_No;
        this->write(tt);
        break;
    case type_No:
        data.remove(0,1);
        Info.No = data;
        tt[0] = type_mac;
        this->write(tt);
        break;
    case type_mac:
        data.remove(0,1);
        Info.Mac = data;
        tt[0] = type_version;
        this->write(tt);

        break;
    case type_version:
        data.remove(0,1);
        Info.Version = data;
        emit updateShow();
        break;
    case type_test:
        break;
    case type_heart:
        break;
    case type_state:
        break;

    default:
        break;
    }
}

void tcpClient::DisConnect()
{
    //断开连接时，发送断开信号
    emit ClientDisConnect(this->clientID,this->peerAddress().toString(),this->peerPort());
}
