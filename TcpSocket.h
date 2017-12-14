#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QDir>
#include <QFile>
#include <QTime>
#include <QObject>
#include <QProcess>
#include <QTcpSocket>
#include <QDataStream>
#include <QHostAddress>
#include <QCryptographicHash>

#define NET "./network/"
#define TMP "./temp/"
#define CON "./config/"

#define ADDR 6000
const quint16 ClientLogin      = 1000;
const quint16 ClientLoginError = 1001;
const quint16 ServerGetHead      = 1002;
const quint16 ClientGetHead      = 1003;
const quint16 SocketDisplay      = 1004;
const quint16 ClientDroped       = 1005;
const quint16 LocalLogin         = 1006;
const quint16 LocalLoginError    = 1007;
const quint16 LocalLoginSuccess  = 1008;

const quint16 HeadMsg      = 2000;
const quint16 HeadError    = 2009;

const quint16 DataMsg      = 2001;
const quint16 DataError    = 2003;
const quint16 DataSuccess  = 2002;

const quint16 CmdMsg       = 2004;
const quint16 CmdSuccess   = 2005;

const quint16 ListMsg      = 2006;
const quint16 ListError    = 2007;



const quint16 GetHeart     = 6000;

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket(QObject *parent = 0);

signals:
    void TransformCmd(quint16 a0,quint16 a1,quint16 cmd,QByteArray data);
public slots:
    void PutBlock(quint16 addr,quint16 cmd,QByteArray data);
    void PutFileHead(QByteArray data);
public:
    qint64 count;
    quint16 TransmitAddr;
private slots:
    void Droped(void);
    void GetBlock(void);
    void GetFileHead(QByteArray msg);
    void GetFileData(QByteArray msg);
    void PutFileData(qint64 numBytes);
    void ExcuteCmd(quint16 addr,quint16 cmd,QByteArray data);
    void Error(QAbstractSocket::SocketError);
    void Display(QByteArray msg);

private:
    QDir *dir;
    QFile *file;
    qint64 LoadSize;
    qint64 BlockSize;
    qint64 BytesRead;
    qint64 BytesToRead;
    qint64 BytesToWrite;
    qint64 BytesWritten;
    QByteArray SendVerify;
    QByteArray RecvVerify;
    bool TransmitShow;
    QString number;
    QString cache;

};

#endif // TCPSOCKET_H
