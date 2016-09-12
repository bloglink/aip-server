#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTime>
#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QHostAddress>

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket(QObject *parent = 0);

signals:
    void TcpQuit(quint16);
    void SendMsg(quint16 port, QByteArray msg);
public slots:
    void WriteMsg(QByteArray msg);
private slots:
    void TcpDisc(void);
    void ReadData(void);
    void ExcuteCmd(QByteArray msg);
    void displayError(QAbstractSocket::SocketError);
private:
    qint64 loadSize;
    qint64 blockSize;
    qint64 bytesToWrite;
    qint64 bytesWritten;

    QByteArray InitMsg;
};

#endif // TCPSOCKET_H
