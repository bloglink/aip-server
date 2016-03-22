#include "tcpclient.h"

tcpClient::tcpClient(QObject *parent,int clientID) :
    QTcpSocket(parent)
{
    this->clientID = clientID;
    isAdmin = false;

    connect(this,SIGNAL(readyRead()),this,SLOT(ReadData()));
    connect(this,SIGNAL(disconnected()),this,SLOT(DisConnect()));
    connect(this,SIGNAL(disconnected()),this,SLOT(deleteLater()));
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.22
  * brief:      读取数据
******************************************************************************/
void tcpClient::ReadData()
{
    this->clientHeart = 0;

    QByteArray data = this->readAll();

    quint8 fun = quint8(data[0]);
    QByteArray tt;

    switch (fun) {
    case sendtype_heart:
        break;
    case sendtype_msg:
        data.remove(0,1);
        emit shareData(data);
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
        emit newRecord(Info.No, state_upper);
        emit updateShow();

        break;
    case type_test:
        emit newRecord(Info.No, state_test);
        break;
    case type_heart:
        break;
    case type_state:
        break;

    default:
        break;
    }

    if (isAdmin == false)
        return;
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.22
  * brief:      断开连接
******************************************************************************/
void tcpClient::DisConnect()
{
    emit newRecord(Info.No, state_lower);
    //断开连接时，发送断开信号
    emit ClientDisConnect(this->clientID);
}
