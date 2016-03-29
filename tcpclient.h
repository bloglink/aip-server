#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QFile>
#include <QTcpSocket>
#include <QDataStream>

#include "my_define.h"

class tcpClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit tcpClient(QObject *parent = 0);

public:
    clientInfo Info;
public slots:
    void SendMessage(quint8 type, QByteArray data);
    void StartTransfer(QString fileName);
    void HeartBeat();
    void HeartClear();
signals:
    void RcvMessage(int index, quint8 type, QByteArray data);
    void ClientDisConnect(int index);
private:
    int heart;
    QFile *file;

    qint64 loadSize;
    qint64 blockSize;
    qint64 totalBytes;
    qint64 bytesToWrite;
    qint64 bytesWritten;
    QByteArray outBlock;

private slots:
    void ReadData();
    void DisConnect();
    void updateClientProgress(qint64 numBytes);  //发送文件内容
};

#endif // TCPCLIENT_H
