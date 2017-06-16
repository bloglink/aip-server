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
#include <QJsonObject>
#include <QJsonDocument>
#include <QUuid>

#include "define.h"

#define NET "./network/"
#define TMP "./temp/"
#define CON "./config/"

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket(QObject *parent = 0);
signals:
    void SendMessage(TcpMap map,QByteArray msg);
    void SendJson(QJsonObject json);
private slots:
    void Init(void);
    void Logout(void);
    void GetBlock(void);
    void GetFileHead(QByteArray msg);
    void GetFileData(QByteArray msg);
    void PutBlock(quint16 addr,quint16 cmd,QByteArray data);
    void PutFileHead(QByteArray data);
    void PutFileData(qint64 numBytes);
    void ExcuteCmd(quint16 addr,quint16 cmd,QByteArray data);
    void Login(QJsonObject json);
    void Error(QAbstractSocket::SocketError);
    void ReadJson(QJsonObject json);
private:

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

    bool isTransmit;
    int TransmitPort;

    QString InitMsg;
    qint64 HeartCount;
    QJsonObject LoginMsg;
    QProcess *proc;
};

#endif // TCPSOCKET_H
