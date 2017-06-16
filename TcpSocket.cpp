#include "TcpSocket.h"

TcpSocket::TcpSocket(QObject *parent) : QTcpSocket(parent)
{
    Init();

    connect(this,SIGNAL(disconnected()),this,SLOT(Logout()));
    connect(this,SIGNAL(readyRead()), this, SLOT(GetBlock()));
    connect(this,SIGNAL(bytesWritten(qint64)),this,SLOT(PutFileData(qint64)));
    connect(this,SIGNAL(error(QAbstractSocket::SocketError)),this,
            SLOT(Error(QAbstractSocket::SocketError)));

    QDir *dir = new QDir;
    if (!dir->exists(QString(TMP)))
        dir->mkdir(QString(TMP));
    if (!dir->exists(QString(NET)))
        dir->mkdir(QString(NET));
}

void TcpSocket::Init()
{
    HeartCount   = 0;
    LoadSize     = 4*1024;
    BlockSize    = 0;
    GetFileByte  = 0;
    GetFileSize  = 0;
    PutFileSize = 0;
    PutFileByte = 0;
    isTransmit = false;
    proc = new QProcess(this);
}



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

void TcpSocket::GetFileData(QByteArray msg)
{
    int ret;
    if (file->isOpen())
        ret = file->write(msg);
    else
        return;
    GetFileByte += ret;
    if (GetFileByte >= GetFileSize) {
        file->seek(0);
        QByteArray md5 = QCryptographicHash::hash(file->readAll(),QCryptographicHash::Md5);
        GetFileByte = 0;
        GetFileSize = 0;
        if (GetFileMD5 == md5) {
            QProcess::execute("sync");
            QString cmd = QString("mv %1 %2").arg(file->fileName()).arg(NET);
            cmd.append(LoginMsg.value("TxUser").toString());
            QProcess::execute(cmd);
            PutBlock(ADDR,FILE_SUCCESS,"NULL");
        } else {
            file->close();
            file->remove();
        }
    }
}

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

void TcpSocket::PutFileHead(QByteArray data)
{
    QString name = data.insert(0,NET);
    file = new QFile(name);
    if (!file->open(QFile::ReadOnly)) {
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

void TcpSocket::PutFileData(qint64)
{
    if (PutFileSize == 0)
        return;
    PutBlock(ADDR,FILE_DATA,file->read(LoadSize));
    PutFileByte += (int)qMin(PutFileSize,LoadSize);
    PutFileSize -= (int)qMin(PutFileSize,LoadSize);
    HeartCount = 0;
    if (PutFileSize==0) {
        file->close();
    }
}

void TcpSocket::ExcuteCmd(quint16 addr, quint16 cmd, QByteArray msg)
{
    HeartCount = 0;
    QJsonObject json;
    json.insert("TxAddress",addr);
    json.insert("RxAddress",this->peerPort());
    json.insert("TxCommand",cmd);
    json.insert("TxMessage",QString(msg));
    if (addr != ADDR) {//数据转发
        emit SendJson(json);
        return;
    }
    switch (cmd) {
    case GUEST_LOGIN:
        json.insert("TxMessage",QString(msg));
        Login(json);
        break;
    case ONLINE_DEVICES://在线列表
        emit SendJson(json);
        break;
    case SERVER_FILES:
        proc->start(QString("ls -F ./network/%1").arg(LoginMsg.value("TxUser").toString()));
        proc->waitForFinished();
        PutBlock(quint16(addr),SERVER_FILES,proc->readAll());
        break;
    case FILE_SUCCESS://发送成功
        break;
    case FILE_ERROR://发送失败
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

void TcpSocket::Login(QJsonObject json)
{
    QString TxUser = json.value("TxMessage").toString().split(" ").at(0);
    QString TxAddrMac = json.value("TxMessage").toString().split(" ").at(1);
    QString TxVersion = json.value("TxMessage").toString().split(" ").at(2);
    QString t = QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss");
    LoginMsg = json;
    LoginMsg.insert("TxTime",t);
    LoginMsg.insert("TxUser",TxUser);
    LoginMsg.insert("TxMessage","Login");
    LoginMsg.insert("TxAddrMac",TxAddrMac);
    LoginMsg.insert("TxVersion",TxVersion);
    LoginMsg.insert("TxAddr",this->peerAddress().toString());
    emit SendJson(LoginMsg);
    QDir *dir = new QDir;
    if (!dir->exists(QString("%1%2").arg(NET).arg(TxUser)))
        dir->mkdir(QString("%1%2").arg(NET).arg(TxUser));
}

void TcpSocket::Logout()
{
    LoginMsg.insert("TxMessage","Logout");
    LoginMsg.insert("TxCommand",GUEST_DROPED);
    emit SendJson(LoginMsg);
    this->deleteLater();
}

void TcpSocket::ReadJson(QJsonObject json)
{
    quint16 TxAddress = json.value("TxAddress").toInt();
    if (TxAddress!=ADDR && TxAddress!=this->peerPort())
        return;
    quint16 RxAddress = json.value("RxAddress").toInt();
    quint16 TxCommand = json.value("TxCommand").toInt();
    PutBlock(RxAddress,TxCommand,json.value("TxMessage").toString().toUtf8());
}

void TcpSocket::Error(QAbstractSocket::SocketError socketError)
{
    qDebug()<<QTime::currentTime().toString()<<errorString();
    if (socketError == QAbstractSocket::RemoteHostClosedError)
        return;
    this->close();
}

