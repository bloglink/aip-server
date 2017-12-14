/*******************************************************************************
 * Copyright (c) 2016,青岛艾普智能仪器有限公司
 * All rights reserved.
 *
 * version:     1.0
 * author:      link
 * date:        2016.03.24
 * brief:       服务器主界面
*******************************************************************************/
#include "CWinServer.h"
#include "ui_CWinServer.h"
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      构造函数
******************************************************************************/
CWinServer::CWinServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CWinServer)
{
    ui->setupUi(this);

    tcp = NULL;
    sql = NULL;

    WinInit();
    KeyInit();
    SqlInit();
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(TcpKeepLive()));
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      析构函数
******************************************************************************/
CWinServer::~CWinServer()
{
    delete ui;
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      初始化列表
******************************************************************************/
void CWinServer::WinInit()
{
    ui->tabTcp->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tabTcp->setRowCount(W_ROW);

    for (int i=0; i<W_ROW*W_COL; i++) {
        ui->tabTcp->setItem(i/W_COL,i%W_COL,new QTableWidgetItem);
        ui->tabTcp->item(i/W_COL,i%W_COL)->setTextAlignment(Qt::AlignCenter);
    }

    QFile file(":/css/aip001.css");
    file.open(QFile::ReadOnly);
    QString qss = QLatin1String(file.readAll());
    this->setStyleSheet(qss);
    this->setPalette(QPalette(QColor("#191919")));
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      初始化按键
******************************************************************************/
void CWinServer::KeyInit()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->KeyPutFile, Qt::Key_A);
    btnGroup->addButton(ui->KeyGetFile, Qt::Key_B);
    btnGroup->addButton(ui->KeySysCmd,  Qt::Key_C);
    btnGroup->addButton(ui->KeyDevCmd,  Qt::Key_D);
    btnGroup->addButton(ui->KeyBegin,   Qt::Key_E);
    btnGroup->addButton(ui->KeyTrust,   Qt::Key_F);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(KeyJudge(int)));
    connect(ui->EditGetFile,SIGNAL(returnPressed()),this,SLOT(TcpGetFileHeadMsg()));
    connect(ui->EditPutFile,SIGNAL(returnPressed()),this,SLOT(TcpPutFileHeadMsg()));
    connect(ui->EditSysCmd,SIGNAL(returnPressed()),this,SLOT(TcpPutCommandMsg()));
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      按键功能
******************************************************************************/
void CWinServer::KeyJudge(int key)
{
    switch (key) {
    case Qt::Key_A:
        TcpPutFileHeadMsg();
        break;
    case Qt::Key_B:
        TcpGetFileHeadMsg();
        break;
    case Qt::Key_C:
        TcpPutCommandMsg();
        break;
    case Qt::Key_D:
        break;
    case Qt::Key_E:
        TcpInit();
        break;
    case Qt::Key_F:
        sql->InsertTrust(ui->tabTcp->currentItem()->text());
        break;
    case Qt::Key_G:
        this->close();
        break;
    default:
        break;
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      更新显示
******************************************************************************/
void CWinServer::DatInit()
{
    for (int i=0; i<W_ROW*W_COL; i++) {
        ui->tabTcp->item(i/W_COL,i%W_COL)->setText("");
    }
    for (int i=0; i<ItemText.size(); i++) {
        QStringList row = ItemText.at(i).split(" ");
        for (int j=0; j<row.size(); j++) {
            ui->tabTcp->item(i,j)->setText(row.at(j));
        }
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      初始化数据库
******************************************************************************/
void CWinServer::SqlInit()
{
    sql = new SqlServer(this);
    sql->Init();
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      开始监听
******************************************************************************/
void CWinServer::TcpInit()
{
    if (tcp == NULL) {
        tcp = new TcpServer(this);
        connect(tcp,SIGNAL(ExcuteCmd(quint16,quint16,QByteArray)),this,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    }
    if (ui->KeyBegin->text() == tr("开始监听")) {
        tcp->listen(QHostAddress::Any,6000);
        ui->KeyBegin->setText(tr("停止监听"));
        timer->start(10000);
    } else {
        timer->stop();
        tcp->AllQuit();
        ui->KeyBegin->setText(tr("开始监听"));
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      心跳
******************************************************************************/
void CWinServer::TcpKeepLive()
{
    for (int i=0; i<tcp->Pool.size(); i++) {
        tcp->Pool.at(i)->count++;
        if (tcp->Pool.at(i)->count++ > 6) {
            tcp->Pool.at(i)->close();
        }
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      TCP写数据
******************************************************************************/
void CWinServer::TcpPutBlock(quint16 addr, quint16 cmd, QByteArray msg)
{
    for (int i=0; i<tcp->Pool.size(); i++) {
        if (tcp->Port.at(i) == addr) {
            tcp->Pool.at(i)->PutBlock(ADDR,cmd,msg);
            break;
        }
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      本地连接
******************************************************************************/
void CWinServer::TcpLogin(quint16 addr,QByteArray msg)
{
    QStringList temp = QString(msg).split(" ");
    QStringList users = sql->ListUsers();
    if (users.contains(temp.at(3)))
        TcpPutBlock(addr,LocalLoginSuccess,"NULL");
    else
        TcpPutBlock(addr,LocalLoginError,"帐号或密码错误\n");
    QStringList user = QString(temp.at(3)).split("@@");
    temp.replace(3,user.at(0));
    TcpClientInit(temp.join(" ").toUtf8());
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      客户端上线
******************************************************************************/
void CWinServer::TcpClientInit(QByteArray msg)
{
    ItemText.append(msg);
    DatInit();

    QStringList temp = QString(msg).split(" ");
    temp.replace(0,"上线");
    sql->InsertState(temp);
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      客户端下线
******************************************************************************/
void CWinServer::TcpClientQuit(quint16 addr)
{
    for (int i=0; i<tcp->Port.size(); i++) {
        if (tcp->Port.at(i) == addr) {
            tcp->Port.removeAt(i);
            tcp->Pool.removeAt(i);
        }
    }
    for (int i=0; i<W_ROW; i++) {
        if (ui->tabTcp->item(i,0)->text().toInt() == addr) {
            QStringList temp = QString(ItemText.at(i)).split(" ");
            temp.replace(0,"下线");
            sql->InsertState(temp);
            ItemText.removeAt(i);
            DatInit();
            break;
        }
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      下发在线列表
******************************************************************************/
void CWinServer::TcpPutListMsg(quint16 addr)
{
    for (int i=0; i<ItemText.size(); i++) {
        QStringList temp = QString(ItemText.at(i)).split(" ");
        if (QString(temp.at(0)).toInt() == addr) {
            QStringList trust = sql->ListTrust();
            if (trust.contains(temp.at(4)))
                TcpPutBlock(addr,ListMsg,(ItemText.join("@@")).toUtf8());
        }
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      发送系统命令
******************************************************************************/
void CWinServer::TcpPutCommandMsg()
{
    TcpPutBlock(CurrentPort(),CmdMsg,ui->EditSysCmd->text().toUtf8());
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      发送文件头
******************************************************************************/
void CWinServer::TcpPutFileHeadMsg(void)
{
    int ret = ui->tabTcp->currentRow();
    if (ret < 0)
        return;
    if (ui->EditPutFile->text().isEmpty())
        return;
    quint16 port = quint16(ui->tabTcp->item(ret,0)->text().toInt());
    QByteArray name = ui->EditPutFile->text().toUtf8();
    for (int i=0; i<tcp->Pool.size(); i++) {
        if (tcp->Port.at(i) == port) {
            tcp->Pool.at(i)->PutFileHead(name);
            break;
        }
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      获取文件头
******************************************************************************/
void CWinServer::TcpGetFileHeadMsg()
{
    TcpPutBlock(CurrentPort(),ServerGetHead,ui->EditGetFile->text().toUtf8());
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      接收数据
******************************************************************************/
void CWinServer::ExcuteCmd(quint16 addr,quint16 cmd, QByteArray msg)
{
    switch (cmd) {
    case ClientLogin:
        TcpClientInit(msg);
        break;
    case ClientLoginError:
        break;
    case LocalLogin:
        TcpLogin(addr,msg);
        break;
    case ListMsg:
        TcpPutListMsg(addr);
        break;
    case ListError:
        break;
    case ClientDroped:
        TcpClientQuit(msg.toInt());
        break;
    case SocketDisplay:
        ui->Text->setText(msg);
        break;
    default:
        qDebug()<<cmd<<msg;
        break;
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      当前行
******************************************************************************/
quint16 CWinServer::CurrentPort()
{
    int ret = ui->tabTcp->currentRow();
    if (ret < 0)
        return -1;
    quint16 port = quint16(ui->tabTcp->item(ret,0)->text().toInt());
    return port;
}
/*******************************************************************************
 *                                  END
*******************************************************************************/
