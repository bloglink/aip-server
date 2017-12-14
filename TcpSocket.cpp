#include "TcpSocket.h"

TcpSocket::TcpSocket(QObject *parent) : QTcpSocket(parent)
{
    count        = 0;
    LoadSize     = 4*1024;;
    BlockSize    = 0;
    BytesRead    = 0;
    BytesToRead  = 0;
    BytesToWrite = 0;
    BytesWritten = 0;
    TransmitShow = false;

    connect(this,SIGNAL(disconnected()),this,SLOT(Droped()));
    connect(this,SIGNAL(readyRead()), this, SLOT(GetBlock()));
    connect(this, SIGNAL(bytesWritten(qint64)),this, SLOT(PutFileData(qint64)));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(Error(QAbstractSocket::SocketError)));

    dir = new QDir;
    if (!dir->exists(QString(TMP)))
        dir->mkdir(QString(TMP));
    if (!dir->exists(QString(NET)))
        dir->mkdir(QString(NET));
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      连接中断
******************************************************************************/
void TcpSocket::Droped()
{
    QByteArray msg;
    msg.append(QString::number(this->peerPort()));
    emit TransformCmd(this->peerPort(),ADDR,ClientDroped,msg);
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      读取数据
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
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      读取文件头
******************************************************************************/
void TcpSocket::GetFileHead(QByteArray msg)
{
    SendVerify = msg.mid(0,16);
    QString temp = msg.remove(0,17);
    QStringList detail = temp.split(" ");
    BytesToRead = detail.at(0).toInt();
    BytesRead = 0;
    file = new QFile(QString(TMP).append(detail.at(1)));
    if(!file->open(QFile::ReadWrite)) {
        PutBlock(ADDR,HeadError,file->errorString().toUtf8());
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      读取文件
******************************************************************************/
void TcpSocket::GetFileData(QByteArray msg)
{
    int ret;
    if (file->isOpen())
        ret = file->write(msg);
    else
        return;
    BytesRead += ret;
    Display(QString("服务器已接收%1%").arg(BytesRead*100/BytesToRead).toUtf8());
    if (BytesRead == BytesToRead) {
        file->seek(0);
        RecvVerify = QCryptographicHash::hash(file->readAll(),QCryptographicHash::Md5);
        file->close();
        BytesRead = 0;
        BytesToRead = 0;
        if (SendVerify == RecvVerify) {
            QProcess::execute("sync");
            cache = QString("%1%2").arg(NET).arg(number);
            if (!dir->exists(cache))
                dir->mkdir(cache);
            QString cmd = QString("mv %1 %2").arg(file->fileName()).arg(cache);
            QProcess::execute(cmd);
            QString wincmd = QString("move %1 %2").arg(file->fileName()).arg(cache);
            QProcess::execute(wincmd);
            Display("服务器接收成功");
        } else {
            QString cmd = QString("rm %1").arg(file->fileName());
            QProcess::execute(cmd);
            Display("服务器接收失败");
        }
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      写数据
******************************************************************************/
void TcpSocket::PutBlock(quint16 addr, quint16 cmd, QByteArray data)
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
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      发送文件头
******************************************************************************/
void TcpSocket::PutFileHead(QByteArray data)
{
    QString name = data;
    file = new QFile(name);
    if (!file->open(QFile::ReadOnly)) {
        PutBlock(ADDR,HeadError,file->errorString().toUtf8());
        Display(QString("打开文件失败:%1").arg(file->errorString()).toUtf8());
        return;
    }
    QByteArray msg;
    msg.append(QCryptographicHash::hash(file->readAll(),QCryptographicHash::Md5));
    file->seek(0);
    msg.append(QString(" %1 ").arg(file->size()));
    msg.append(name.right(name.size()- name.lastIndexOf('/')-1));
    PutBlock(ADDR,HeadMsg,msg);
    BytesToWrite = file->size();
    BytesWritten = 0;
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      发送文件数据
******************************************************************************/
void TcpSocket::PutFileData(qint64 numBytes)
{
    if (BytesToWrite == 0)
        return;
    BytesWritten += (int)numBytes;
    PutBlock(quint16(ADDR),quint16(DataMsg),file->read(LoadSize));
    BytesToWrite -= (int)qMin(BytesToWrite,LoadSize);
    Display(QString("服务器已发送%1%").arg(100-BytesToWrite*100/file->size()).toUtf8());
    if (BytesToWrite == 0)
        file->close();
    count = 0;
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      命令执行
******************************************************************************/
void TcpSocket::ExcuteCmd(quint16 addr, quint16 cmd, QByteArray data)
{
    count = 0;
    if (addr != ADDR) {
        emit TransformCmd(peerPort(),addr,cmd,data);
        return;
    }
    switch (cmd) {
    case ClientLogin://上线初始化
        number = data;
        number = number.split(" ").at(0);
        data.insert(0,QString("%1 ").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss")));
        data.insert(0,QString("%1 ").arg(peerAddress().toString()));
        data.insert(0,QString("%1 ").arg(peerPort()));
        emit TransformCmd(peerPort(),ADDR,ClientLogin,data);
        TransmitAddr = this->peerPort();
        break;
    case ClientLoginError:
        break;
    case LocalLogin:
        number = data;
        number = number.split(" ").at(0);
        number = number.split("@@").at(0);
        data.insert(0,QString("%1 ").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss")));
        data.insert(0,QString("%1 ").arg(peerAddress().toString()));
        data.insert(0,QString("%1 ").arg(peerPort()));
        emit TransformCmd(this->peerPort(),addr,cmd,data);
        break;
    case ServerGetHead://获取文件头命令
        this->PutBlock(ADDR,ServerGetHead,data);
        break;
    case ClientGetHead:
        PutFileHead(data);
        break;
    case HeadMsg://收取文件头
        GetFileHead(data);
        break;
    case HeadError://获取文件头失败
        Display(data);
        break;
    case DataMsg:
        GetFileData(data);
        break;
    case DataError:
        Display(data);
        break;
    case DataSuccess:
        Display("服务器发送成功");
        break;
    case CmdMsg:
        break;
    case CmdSuccess:
        Display(data);
        break;
    case ListMsg:
        TransmitShow = true;
        emit TransformCmd(this->peerPort(),addr,cmd,data);
        break;
    case ListError:
        TransmitShow = false;
        break;
    case GetHeart://心跳
        PutBlock(quint16(ADDR),quint16(GetHeart),"NULL");
        break;
    default:
        break;
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      输出错误信息
******************************************************************************/
void TcpSocket::Error(QAbstractSocket::SocketError socketError)
{
    if (socketError == QAbstractSocket::RemoteHostClosedError)
        return;
    Display(errorString().toUtf8());
    this->close();
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      显示信息
******************************************************************************/
void TcpSocket::Display(QByteArray msg)
{
    if (TransmitShow)
        this->PutBlock(ADDR,SocketDisplay,msg);
    if (TransmitAddr != this->peerPort())
        emit TransformCmd(peerPort(),TransmitAddr,SocketDisplay,msg);
    emit TransformCmd(peerPort(),ADDR,SocketDisplay,msg);
}

