#include "TcpSocket.h"

TcpSocket::TcpSocket(QObject *parent) : QTcpSocket(parent)
{
    SockectVersion= 1;
    HeartCount   = 0;
    LoadSize     = 4*1024;;
    BlockSize    = 0;
    GetFileByte    = 0;
    GetFileSize  = 0;
    PutFileSize = 0;
    PutFileByte = 0;
    isTransmit = false;

    connect(this,SIGNAL(disconnected()),this,SLOT(Droped()));
    connect(this,SIGNAL(readyRead()), this, SLOT(GetBlock()));
    connect(this,SIGNAL(bytesWritten(qint64)),this,SLOT(PutFileData(qint64)));
    connect(this,SIGNAL(error(QAbstractSocket::SocketError)),this,
            SLOT(Error(QAbstractSocket::SocketError)));

    dir = new QDir;
    if (!dir->exists(QString(TMP)))
        dir->mkdir(QString(TMP));
    if (!dir->exists(QString(NET)))
        dir->mkdir(QString(NET));

}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.07.16
 * brief:       连接中断
******************************************************************************/
void TcpSocket::Droped()
{
    TcpMap map;
    map.insert("TxAddress",ADDR);
    map.insert("RxAddress",this->peerPort());
    map.insert("TxCommand",GUEST_DROPED);
    emit SendMessage(map,NULL);
    this->deleteLater();
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.07.16
 * brief:       读取数据
******************************************************************************/
void TcpSocket::GetBlock()
{
    quint16 addr;
    quint16 cmd;
    QByteArray msg;
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_8);
    while (1) {
        if (BlockSize == 0) {
            if (this->bytesAvailable() < qint64(sizeof(qint64)))
                return;
            in >> BlockSize;
        }
        if (BlockSize > 8*1024) {
            qDebug() << "block size overflow";
            this->abort();
            return;
        }
        if (this->bytesAvailable() < BlockSize)
            return;
        in >> addr >> cmd >> msg;
        ExcuteCmd(addr,cmd,msg);
        BlockSize = 0;
    }
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.07.16
 * brief:       读取文件头
******************************************************************************/
void TcpSocket::GetFileHead(QByteArray msg)
{
    GetFileMD5 = msg.mid(0,16);
    QString temp = msg.remove(0,17);
    QStringList detail = temp.split(" ");
    GetFileSize = detail.at(0).toInt();
    GetFileName = detail.at(1).toUtf8();
    GetFileByte = 0;
    file = new QFile(QString(TMP).append(GetFileName));
    if(!file->open(QFile::ReadWrite)) {
        PutBlock(ADDR,FILE_HEAD_ERROR,file->errorString().toUtf8());
    }
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.07.16
 * brief:       读取文件
******************************************************************************/
void TcpSocket::GetFileData(QByteArray msg)
{
    int ret;
    if (file->isOpen())
        ret = file->write(msg);
    else
        return;
    GetFileByte += ret;
    Display(QString("服务器已接收%1%").arg(GetFileByte*100/GetFileSize).toUtf8());
    if (GetFileByte >= GetFileSize) {
        file->seek(0);
        QByteArray md5 = QCryptographicHash::hash(file->readAll(),QCryptographicHash::Md5);
        GetFileByte = 0;
        GetFileSize = 0;
        if (GetFileMD5 == md5) {
            QProcess::execute("sync");
            QString cmd = QString("mv %1 %2").arg(file->fileName()).arg(NET);
            QProcess::execute(cmd);
            Display("服务器接收成功");
        } else {
            file->close();
            file->remove();
            Display("服务器接收失败");
        }
    }
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.07.16
 * brief:       写数据
******************************************************************************/
void TcpSocket::PutBlock(quint16 addr,quint16 cmd,QByteArray data)
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<(qint64)0<<quint16(addr)<<quint16(cmd)<<data;
    out.device()->seek(0);
    out<<(qint64)(msg.size()-sizeof(qint64));
    this->write(msg);
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.07.16
 * brief:       发送文件头
******************************************************************************/
void TcpSocket::PutFileHead(QByteArray data)
{
    QString name = data;
    file = new QFile(name);
    if (!file->open(QFile::ReadOnly)) {
        Display(file->errorString().toUtf8());
        return;
    }
    PutFileMD5 = QCryptographicHash::hash(file->readAll(),QCryptographicHash::Md5);
    file->seek(0);
    PutFileSize = file->size();
    PutFileByte = 0;
    PutFileName = name.right(name.size()- name.lastIndexOf('/')-1).toUtf8();
    QByteArray msg;
    msg.append(PutFileMD5);
    msg.append(QString(" %1 ").arg(PutFileSize));
    msg.append(PutFileName);
    PutBlock(ADDR,FILE_HEAD,msg);
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.07.16
 * brief:       发送文件数据
******************************************************************************/
void TcpSocket::PutFileData(qint64)
{
    if (PutFileSize == 0)
        return;
    PutBlock(ADDR,FILE_DATA,file->read(LoadSize));
    PutFileByte += (int)qMin(PutFileSize,LoadSize);
    PutFileSize -= (int)qMin(PutFileSize,LoadSize);
    Display(QString("服务器已发送%1%").arg(PutFileByte*100/(PutFileSize+PutFileByte)).toUtf8());
    HeartCount = 0;
    if (PutFileSize==0) {
        file->close();
    }
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.16
 * brief:       命令执行
******************************************************************************/
void TcpSocket::ExcuteCmd(quint16 addr, quint16 cmd, QByteArray msg)
{
    HeartCount = 0;
    TcpMap map;
    map.insert("TxAddress",addr);
    map.insert("RxAddress",this->peerPort());
    map.insert("TxCommand",cmd);

    QUrl url = UrlInit;
    url.setPort(addr);
    url.setQuery(QString::number(cmd));
    url.setFragment(msg);

    if (addr != ADDR) {//数据转发
        emit SendMessage(map,msg);
        return;
    }
    switch (cmd) {
    case GUEST_LOGIN:
    case ADMIN_LOGIN://登录
        GuestLogin(map,msg);
//        Login(data);
        break;
    case ONLINE_DEVICES://在线列表
    case SHELL_DAT://执行命令结果
        emit SendMessage(map,msg);
        break;
    case FILE_SUCCESS://发送成功
        Display("服务器发送成功");
        break;
    case FILE_ERROR://发送失败
        Display(msg);
        break;
    case GUEST_PUT_HEAD://获取客户文件
        this->PutBlock(ADDR,GUEST_PUT_HEAD,msg);
        break;
    case GUEST_GET_HEAD://客户获取文件
        PutFileHead(msg);
        break;
    case FILE_HEAD://收取文件头
        GetFileHead(msg);
        break;
    case FILE_HEAD_ERROR://获取文件失败
        Display(msg);
        break;
    case FILE_DATA://获取文件内容
        GetFileData(msg);
        break;
    case HEART_BEAT://心跳
        PutBlock(ADDR,HEART_BEAT,NULL);
        break;
    default:
        break;
    }
}

void TcpSocket::GuestLogin(TcpMap map, QByteArray msg)
{
    msg.append(" ");
    msg.append(this->peerAddress().toString());
    emit SendMessage(map,msg);
}

void TcpSocket::ReadMessage(TcpMap map, QByteArray msg)
{
    quint16 TxAddress = map.value("TxAddress");
    quint16 RxAddress = map.value("RxAddress");
    quint16 TxCommand = map.value("TxCommand");
    if (TxAddress!=ADDR && TxAddress!=this->peerPort())
        return;
    switch (TxCommand) {
    case FILE_HEAD:
        PutFileHead(msg);
        break;
    case BUILD_TRANSMIT:
        isTransmit = true;
        TransmitPort = msg.toInt();
        break;
    case BREAK_TRANSMIT:
        isTransmit = false;
        break;
    case HEART_BEAT://心跳
        HeartCount++;
        if (HeartCount > 5)
            this->disconnectFromHost();
        break;
    default:
        PutBlock(RxAddress,TxCommand,msg);
        break;
    }
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.07.16
 * brief:       输出错误信息
******************************************************************************/
void TcpSocket::Error(QAbstractSocket::SocketError socketError)
{
    if (socketError == QAbstractSocket::RemoteHostClosedError)
        return;
    Display(errorString().toUtf8());
    this->close();
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.07.16
 * brief:       显示信息
******************************************************************************/
void TcpSocket::Display(QByteArray msg)
{
    TcpMap map;
    map.insert("TxAddress",ADDR);
    map.insert("RxAddress",this->peerPort());
    map.insert("TxCommand",GUEST_DISPLAY);
    emit SendMessage(map,msg);

    if (isTransmit) {
        map["TxAddress"] = TransmitPort;
        emit SendMessage(map,msg);
    }
}
