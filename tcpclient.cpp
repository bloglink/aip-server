#include "tcpclient.h"

tcpClient::tcpClient(QObject *parent) :
    QTcpSocket(parent)
{
    blockSize = 0;
    connect(this,SIGNAL(readyRead()),this,SLOT(ReadData()));
    connect(this,SIGNAL(disconnected()),this,SLOT(DisConnect()));
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.22
  * brief:      读取数据
******************************************************************************/
void tcpClient::ReadData()
{

    QByteArray data;
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_8);

    while (1) {
        if (blockSize == 0) {
            if (this->bytesAvailable() < sizeof(quint16))
                return;
        }
        in >> blockSize;
        if (this->bytesAvailable() < blockSize)
            return;

        in >> data;

        quint8 fun = quint8(data[0]);

        if (fun == type_heart)
            Info.heart = 0;
        else
            emit RcvData(Info.ID.toInt(), data);

        blockSize = 0;
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.22
  * brief:      断开连接
******************************************************************************/
void tcpClient::DisConnect()
{
    Info.isFree = true;
    Info.isInit = false;
    emit ClientDisConnect(Info.ID.toInt());
}
