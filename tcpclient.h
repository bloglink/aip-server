#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QDesktopWidget>
#include <QFile>
#include <QTextCodec>
#include <QMessageBox>
#include <QAbstractButton>
#include <QCoreApplication>
#include <QFileDialog>
#include <QTime>
#include <QProcess>
#include <QDir>
#include <QApplication>

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
    void StartTransfer();    //开始传输文件
    void SendMessage(quint8 type, QByteArray data);
signals:
    void RcvData(int index, QByteArray data);
    void RcvMessage(int index, quint8 type, QByteArray data);
    void ClientDisConnect(int index);
private:
    QFile *localFile;

    qint64 loadSize;
    qint64 blockSize;
    qint64 totalBytes;
    qint64 bytesToWrite;
    qint64 bytesWritten;
    QByteArray outBlock;


private slots:
    void ReadData();
    void DisConnect();
//    void startSend(QByteArray data);

    void updateClientProgress(qint64 numBytes);  //发送文件内容



};

#endif // TCPCLIENT_H
