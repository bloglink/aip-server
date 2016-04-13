#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QFile>
#include <QTcpSocket>
#include <QDataStream>
#include <QCryptographicHash>

#include "my_define.h"

class tcpClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit tcpClient(QObject *parent = 0,int ClientIndex = 0);

public:
    clientInfo Info;
public slots:
    void SendMessage(quint64 type, quint64 origin, QByteArray data);
    void StartTransfer(QString fileName);
    void HeartBeat();
    void HeartClear();
    void displayError(QAbstractSocket::SocketError);
signals:
    void RcvMessage(int index, quint64 type, quint64 target, QByteArray data);
    void ClientDisConnect(int index);
private:
    int index;
    int heart;
    QFile *file;

    qint64 loadSize;
    qint64 blockSize;
    qint64 totalBytes;
    qint64 bytesToWrite;
    qint64 bytesWritten;
    QByteArray msg;

private slots:
    void ReadData();
    void DisConnect();
    void updateClientProgress(qint64 numBytes);  //发送文件内容
};

#endif // TCPCLIENT_H
