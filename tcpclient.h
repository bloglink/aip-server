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
#include <QSound>
#include <QApplication>
#include <QStyleFactory>
#include <QInputDialog>
#include <QDebug>
class tcpClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit tcpClient(QObject *parent = 0,int clientID=0);
    
private:
    int clientID;

signals:
    void ClientReadData(int clientID,QString IP,int Port,QByteArray data);
    void ClientDisConnect(int clientID,QString IP,int Port);

private slots:
    void ReadData();
    void DisConnect();

public slots:
    
};

#endif // TCPCLIENT_H
