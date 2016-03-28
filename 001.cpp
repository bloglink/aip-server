#include "testclient.h"

testClient::testClient(QObject *parent) :
    QTcpSocket(parent)
{
    //发送变量初始化
    num = -1;
    loadSize = 100*1024;
    openList.clear();

    //接收变量初始化
    TotalRecvBytes = 0;
    bytesReceived = 0;
    fileNameSize = 0;
    recvFileNum = 0;
    totalRecvSize = 0;
    totalBytesReceived = 0;
    recvNum = 0;
    receiveClass = 255;
    fileClass = 0;
    for(int i=0; i<3; i++)
        isReceived[i] = false;

    //连接信号和槽
    connect(this, SIGNAL(readyRead()), this, SLOT(recvData()));
    connect(this, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
    connect(this, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(newState(QAbstractSocket::SocketState)));
    connect(this, SIGNAL(bytesWritten(qint64)), this, SLOT(updateClientProgress(qint64)));
}


void testClient::recvData()     //接收数据，服务器只接收客户端的一个结果文件
{
    QDataStream in(this); //本地数据流
    in.setVersion(QDataStream::Qt_4_8 ); //设置流版本，以防数据格式错误

    QString unit;
    qint32 msg;

    if(bytesReceived <= (sizeof(quint8)))
    {
        if(this->bytesAvailable() >= (sizeof(quint8)))
        {
            in >> receiveClass;
        }
        switch(receiveClass)
        {
        case sendtype_file:     //接收文件
            bytesReceived += sizeof(quint8);
            qDebug() << "bytesReceived: " << bytesReceived;
            break;

        case sendtype_msg:
            in >> msg;

            if(msg == 0)    //接收文件列表
            {
                emit outputlog(tr("client %1 have received list file")
                               .arg(this->peerAddress().toString()));

            }
            else if(msg == 1)   //接收文件
            {
                emit outputlog(tr("client %1 have received file(s)")
                               .arg(this->peerAddress().toString()));

            }
            return;

        default:
            return;
        }
    }

    if(bytesReceived >= (sizeof(quint8)) && bytesReceived <= (sizeof(quint8) + sizeof(qint64)*2 + sizeof(quint16)))   //开始接收文件，先接受报头
    {
        //收3个int型数据，分别存储总长度、文件类型和文件名长度
        if( ( this->bytesAvailable() >= (sizeof(qint64)*2 + sizeof(quint16)) ) && (fileNameSize == 0) )
        {
            in >> TotalRecvBytes >> fileClass >> fileNameSize;

            bytesReceived += sizeof(qint64)*2;  //收到多少字节
            bytesReceived += sizeof(quint16);

            if(fileClass == filetype_result)
            {
                recvNum = 1;
                recvFileNum = 1;
                totalRecvSize = TotalRecvBytes;  //只有一个文件，文件总大小为该文件发送大小
                totalBytesReceived += sizeof(qint64)*2;
                totalBytesReceived += sizeof(quint16);
                totalBytesReceived += sizeof(quint8);

                emit newClientInfo(tr("receiving result"),num,4);
            }
            else
            {
                QMessageBox::warning(NULL,tr("WARNING"),tr("client %1 send wrong type of file to server")
                                     .arg(this->peerAddress().toString()));
                return;
            }

        }
        //接着收文件名并建立文件
        if((this->bytesAvailable() >= fileNameSize)&&(fileNameSize != 0))
        {
            in >> receivedFileName;
            bytesReceived += fileNameSize;

            totalBytesReceived += fileNameSize;

            QString receiveFilePath = receive_path + "/" + receivedFileName;    //接收文件路径

            emit outputlog(tr("receive from client %1\nsave as:%2")
                           .arg(this->peerAddress().toString())
                           .arg(receiveFilePath));

            //建立文件
            receivedFile = new QFile(receiveFilePath);

            if (!receivedFile->open(QFile::WriteOnly ))
            {
                QMessageBox::warning(NULL, tr("WARNING"),
                                     tr("cannot open file %1:\n%2.").arg(receivedFileName).arg(receivedFile->errorString()));
                return;
            }
        }
        else
        {
            return;
        }
    }

    //一个文件没有接受完
    if (bytesReceived < TotalRecvBytes)
    {
        //可用内容比整个文件长度-已接收长度短，则全部接收并写入文件
        qint64 tmp_Abailable = this->bytesAvailable();
        if(tmp_Abailable <= (TotalRecvBytes - bytesReceived))
        {
            bytesReceived += tmp_Abailable;
            totalBytesReceived += tmp_Abailable;
            inBlock = this->readAll();
            receivedFile->write(inBlock);
            inBlock.resize(0);
            tmp_Abailable = 0;
        }
        //可用内容比整个文件长度-已接收长度长，则接收所需内容，并写入文件
        else
        {
            inBlock = this->read(TotalRecvBytes - bytesReceived);

            if(fileClass == filetype_wavfile)
            {
                totalBytesReceived += (TotalRecvBytes - bytesReceived);
            }
            bytesReceived = TotalRecvBytes;
            receivedFile->write(inBlock);
            inBlock.resize(0);
            tmp_Abailable = 0;
        }
    }

    emit updateProgressSignal(totalBytesReceived,totalRecvSize,num);   //更新发送进度信号


    //善后：一个文件全部收完则重置变量关闭文件流，删除指针
    if (bytesReceived == TotalRecvBytes)
    {
        //变量重置
        TotalRecvBytes = 0;
        bytesReceived = 0;
        fileNameSize = 0;
        receiveClass = 255;
        receivedFile->close();
        delete receivedFile;

        //输出信息
        emit outputlog(tr("Have received file: %1 from client %2")
                       .arg(receivedFileName)
                       .arg(this->peerAddress().toString()));   //log information

        //全部文件接收完成
        if(recvNum == recvFileNum)
        {
            //变量重置
            recvFileNum = 0;
            recvNum = 0;
            totalBytesReceived = 0;
            totalRecvSize = 0;

            emit outputlog(tr("Receive all done!"));
        }

        if(fileClass == filetype_result)
        {
            emit newClientInfo(tr("Evaluating"),num,4);
        }
    }
}


void testClient::newState(QAbstractSocket::SocketState state)    //新状态
{
    emit newClient(this->peerAddress().toString(), (int)state, num);
}


void testClient::startTransfer(const quint16 type)    //开始传输文件
{
    TotalSendBytes = 0;    //总共需发送的字节数
    bytesWritten = 0;       //已发送字节数
    bytesToWrite = 0;       //待发送字节数

    //开始传输文件信号
    emit outputlog(tr("start sending file to client: %1\n filename: %2")
                   .arg(this->peerAddress().toString())
                   .arg(sendFileName));

    sendFile = new QFile(sendFilePath);
    if (!sendFile->open(QFile::ReadOnly ))  //读取发送文件
    {
        QMessageBox::warning(NULL, tr("WARNING"),
                             tr("can not read file %1:\n%2.")
                             .arg(sendFilePath)
                             .arg(sendFile->errorString()));
        return;
    }
    TotalSendBytes = sendFile->size();
    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_4_8);

    //写入发送类型，数据大小，文件类型，文件名大小，文件名
    sendOut << quint8(0) << qint64(0) << quint16(0) << qint64(0) << sendFileName;
    TotalSendBytes +=  outBlock.size();
    sendOut.device()->seek(0);
    sendOut << quint8(sendtype_file)<< TotalSendBytes << quint16(type)
            << qint64((outBlock.size() - sizeof(qint64) * 2) - sizeof(quint16) - sizeof(quint8));

    this->writeData(outBlock,outBlock.size());
    //this->flush();
    bytesToWrite = TotalSendBytes - outBlock.size();

    outBlock.resize(0);
}

void testClient::updateClientProgress(qint64 numBytes)  //发送文件内容
{
    if(TotalSendBytes == 0)
        return;

    bytesWritten += (int)numBytes;
    proBarValue += (int)numBytes;

    emit updateProgressSignal(proBarValue,proBarMax,num);   //更新发送进度信号

    if (bytesToWrite > 0)
    {
        outBlock = sendFile->read(qMin(bytesToWrite, loadSize));
        bytesToWrite -= (int)this->writeData(outBlock,outBlock.size());
        outBlock.resize(0);
    }
    else
    {
        sendFile->close();

        //结束传输文件信号
        if(TotalSendBytes < 1024)
        {
            emit outputlog(tr("finish sending file to client: %1\n filename: %2 %3B")
                           .arg(this->peerAddress().toString())
                           .arg(sendFileName)
                           .arg(TotalSendBytes));
        }
        else if(TotalSendBytes < 1024*1024)
        {
            emit outputlog(tr("finish sending file to client: %1\n filename: %2 %3KB")
                           .arg(this->peerAddress().toString())
                           .arg(sendFileName)
                           .arg(TotalSendBytes / 1024.0));
        }
        else
        {
            emit outputlog(tr("finish sending file to client: %1\n filename: %2 %3MB")
                           .arg(this->peerAddress().toString())
                           .arg(sendFileName)
                           .arg(TotalSendBytes / (1024.0*1024.0)));
        }

        if(sendNum < openList.size())   //还有文件需要发送
        {
            if(sendNum == 0)
            {
                //QFile::remove(sendFilePath);    //删除列表文件
                proBarMax = totalSendSize;
                proBarValue = 0;
            }
            sendFilePath = openList[sendNum].filePath;
            sendFileName = openList[sendNum].fileName;
            sendNum++;
            startTransfer(filetype_wavfile);
        }
        else    //发送结束
        {
            emit newClientInfo(tr("send complete"),num,4);

            TotalSendBytes = 0;    //总共需发送的字节数
            bytesWritten = 0;       //已发送字节数
            bytesToWrite = 0;       //待发送字节数
        }
    }
}

void testClient::prepareTransfer(std::vector<openFileStruct>& openFileList,int testtype_t)    //准备传输文件
{
    if(openFileList.size() == 0)    //没有文件
    {
        return;
    }

    testtype_now = testtype_t;
    isSendKeyword = false;
    for(int i=0; i<2; i++)
        isReceived[i] = false;

    openList.clear();
    openList.assign(openFileList.begin(),openFileList.end());   //拷贝文件列表

    QString sendFileListName = "sendFileList.txt";
    QString sendFileListPath = temp_Path + "/" + sendFileListName;

    getSendFileList(sendFileListPath);     //在指定路径生成发送文件列表

    emit newClientInfo(tr("sending test files"),num,4);   //更新主窗口测试阶段
    sendFilePath = sendFileListPath;
    sendFileName = sendFileListName;
    sendNum = 0;    //发送到第几个文件

    proBarMax = getSendTotalSize();
    proBarValue = 0;

    startTransfer(filetype_list);    //开始传输文件
}

void testClient::getSendFileList(QString path)     //在指定路径生成发送文件列表
{
    sendFileNum = openList.size();    //记录发送文件个数
    totalSendSize = 0;  //记录发送文件总大小

    for(int i = 0; i < sendFileNum; i++)
    {
        sendFileName = openList[i].fileName;
        sendFilePath = openList[i].filePath;

        totalSendSize += getSendTotalSize();
    }

    FILE *fp;
    fp = fopen(path.toLocal8Bit().data(),"w");

    fprintf(fp,"%d\n",sendFileNum);
    fprintf(fp,"%d\n",totalSendSize);

    for(int i = 0; i < sendFileNum; i++)
    {
        fprintf(fp,"%s\n",openList[i].fileName.toLocal8Bit().data());
    }

    fclose(fp);
}

quint64 testClient::getSendTotalSize()    //获取sendFilePath对应文件加上头的大小
{
    int totalsize;
    //计算列表文件及文件头总大小
    QFile *file = new QFile(sendFilePath);
    if (!file->open(QFile::ReadOnly ))  //读取发送文件
    {
        QMessageBox::warning(NULL, tr("WARNING"),
                             tr("can not read file %1:\n%2.")
                             .arg(sendFilePath)
                             .arg(file->errorString()));
        return 0;
    }

    totalsize = file->size();  //文件内容大小
    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_4_8);
    //写入发送类型，数据大小，文件类型，文件名大小，文件名
    sendOut << quint8(0) << qint64(0) << quint16(0) << qint64(0) << sendFileName;
    totalsize +=  outBlock.size();  //文件头大小

    file->close();

    outBlock.resize(0);

    return totalsize;
}

void testClient::clientConnected()
{

}
void testClient::clientDisconnected()
{

}
