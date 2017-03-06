#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QUrl>
#include <QDir>
#include <QFile>
#include <QTimer>
#include <QObject>
#include <QProcess>
#include <QSettings>
#include <QHostInfo>
#include <QTcpSocket>
#include <QDataStream>
#include <QNetworkInterface>
#include <QCryptographicHash>
#include <QNetworkAccessManager>
#include <QFileInfo>
#include <QDateTime>

#include "define.h"

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket(QObject *parent = 0);
signals:
    void SendMessage(TcpMap map,QByteArray msg);
public slots:
    void PutBlock(quint16 addr,quint16 cmd,QByteArray data);
public:
    qint64 HeartCount;
    QUrl UrlInit;
private slots:
    void Droped(void);
    void GetBlock(void);
    void GetFileHead(QByteArray msg);
    void GetFileData(QByteArray msg);
    void PutFileHead(QByteArray data);
    void PutFileData(qint64 numBytes);
    void ExcuteCmd(quint16 addr,quint16 cmd,QByteArray data);
    void GuestLogin(TcpMap map,QByteArray msg);
    void Error(QAbstractSocket::SocketError);
    void Display(QByteArray msg);
    void ReadMessage(TcpMap map,QByteArray msg);

private:
    QDir *dir;
    QFile *file;
    qint64 LoadSize;
    qint64 BlockSize;

    QByteArray RecvVerify;
    QString cache;

    qint64 PutFileSize;
    qint64 PutFileByte;
    QByteArray PutFileMD5;
    QByteArray PutFileName;

    qint64 GetFileByte;
    qint64 GetFileSize;
    QByteArray GetFileMD5;
    QByteArray GetFileName;

    quint8 SockectVersion;
    bool isTransmit;
    int TransmitPort;
};

#endif // TCPSOCKET_H
