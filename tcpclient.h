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
#include <my_define.h>
#include <QDataStream>
class tcpClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit tcpClient(QObject *parent = 0);

public:
    clientInfo Info;
    
signals:
    void RcvData(int index, QByteArray data);
    void ClientDisConnect(int clientID);
private slots:
    void ReadData();
    void DisConnect();

private:
    quint16 blockSize;


    //发送文件所需变量
    qint64  loadSize;          //每次接收的数据块大小

    qint64  TotalSendBytes;        //总共需发送的字节数
    qint64  bytesWritten;      //已发送字节数
    qint64  bytesToWrite;      //待发送字节数
    QString sendFileName;          //待发送的文件的文件名
    QString sendFilePath;          //待发送的文件的文件路径
    QFile *sendFile;          //待发送的文件
    QByteArray outBlock;       //缓存一次发送的数据
    int sendNum;        //记录当前发送到第几个文件
    int sendFileNum;    //记录发送文件个数
    int totalSendSize;  //记录发送文件总大小
    quint64 proBarMax;  //发送进度条最大值
    quint64 proBarValue;    //进度条当前值

    std::vector<openFileStruct> openList;   //发送文件列表

    //接收文件用变量
    quint8 receiveClass;        //0：文件，1：开始测试，20：客户端接收到文件反馈
    quint16 fileClass;          //待接收文件类型
    qint64 TotalRecvBytes;          //总共需接收的字节数
    qint64 bytesReceived;       //已接收字节数
    qint64 fileNameSize;        //待接收文件名字节数
    QString receivedFileName;   //待接收文件的文件名
    QFile *receivedFile;        //待接收文件
    QByteArray inBlock;         //接收临时存储块
    qint64 totalBytesReceived;  //接收的总大小
    qint32 recvNum;       //现在接收的是第几个
    quint64 recvFileNum;         //接收文件个数
    quint64 totalRecvSize;    //接收文件总大小
    bool isReceived[3];     //是否收到客户端的接收文件反馈

    int testtype_now;
    bool isSendKeyword;
    
};

#endif // TCPCLIENT_H
