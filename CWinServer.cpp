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
    WinInit();
    KeyInit();
    SeverInit();
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
    ui->tabTcp->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    ui->tabTcp->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tabTcp->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Fixed);
    ui->tabTcp->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
    ui->tabTcp->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);
    ui->tabTcp->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Fixed);

    ui->tabTcp->setRowCount(W_ROW);

    for (int i=0; i<W_ROW*W_COL; i++) {
        tableItem.append(new QTableWidgetItem);
        tableItem[i]->setTextAlignment(Qt::AlignCenter);
        ui->tabTcp->setItem(i/W_COL,i%W_COL,tableItem[i]);
    }
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
    btnGroup->addButton(ui->KeyFile, Qt::Key_A);
    btnGroup->addButton(ui->KeyCmd,  Qt::Key_B);
    btnGroup->addButton(ui->KeyBegin,Qt::Key_C);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(KeyJudge(int)));
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
        break;
    case Qt::Key_B:
        break;
    case Qt::Key_C:
        if (ui->KeyBegin->text() == tr("开始监听")) {
            server->listen(QHostAddress::Any,6000);
            ui->KeyBegin->setText(tr("停止监听"));
        } else {
            server->AllQuit();
            ui->KeyBegin->setText(tr("开始监听"));
        }
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
        tableItem[i]->setText("");
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
  * brief:      初始化Sever
******************************************************************************/
void CWinServer::SeverInit()
{
    server = new TcpServer(this);
    connect(server,SIGNAL(SendMsg(quint16,QByteArray)),
            this,SLOT(RecvMsg(quint16,QByteArray)));
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      接收数据
******************************************************************************/
void CWinServer::RecvMsg(quint16 port, QByteArray msg)
{
    switch (msg.at(0)) {
    case 'A':
        //        AckSend();
        break;
    case 'B'://初始化
        ItemText.append(msg.remove(0,2));
        DatInit();
        break;
    case 'C'://接收文件头
        break;
    case 'D'://接收文件
        break;
    case 'E'://发送成功
        //        emit SendOk("发送成功");
        break;
    case 'N'://发送失败
        //        emit SendOk("发送失败");
        break;
    case 'Q'://客户退出
        for (int i=0; i<W_ROW; i++) {
            if (ui->tabTcp->item(i,0)->text().toInt() == port) {
                ItemText.removeAt(i);
                DatInit();
                break;
            }
        }
        break;
    default:
        break;
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      发送数据
******************************************************************************/
void CWinServer::SendMsg(quint16 port, QByteArray msg)
{
    for (int i=0; i<server->Pool.size(); i++) {
        if (server->Port.at(i) == port) {
            server->Pool.at(i)->WriteMsg(msg);
            break;
        }
    }
}
/*******************************************************************************
 *                                  END
*******************************************************************************/


