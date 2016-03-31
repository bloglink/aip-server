/*******************************************************************************
 * Copyright (c) 2016,青岛艾普智能仪器有限公司
 * All rights reserved.
 *
 * version:     1.0
 * author:      link
 * date:        2016.03.22
 * brief:       主界面
*******************************************************************************/
#include "w_home.h"
#include "ui_w_home.h"
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      构造函数
******************************************************************************/
w_Home::w_Home(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::w_Home)
{
    int i;
    ui->setupUi(this);

    isServerOn = false;
    page = 0;
    sysStep = 0;

    QStringList str;
    str.append(tr("ID"));
    str.append(tr("IP地址"));
    str.append(tr("出厂编号"));
    str.append(tr("MAC地址"));
    str.append(tr("上线时间"));
    str.append(tr("软件版本"));

    ui->tableWidget->setColumnCount(str.size());
    ui->tableWidget->setHorizontalHeaderLabels(str);
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setRowCount(W_ROW);

    for (i=0; i<W_ROW*W_COL; i++) {
        pItem.append(new QTableWidgetItem);
        pItem[i]->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i/W_COL,i%W_COL,pItem[i]);
    }

    QTimer *timer = new QTimer(this);
    connect (timer,SIGNAL(timeout()),this,SLOT(system()));
    timer->start(50);
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      退出
******************************************************************************/
w_Home::~w_Home()
{
    sql.DisConnect();
    delete ui;
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      系统节拍
******************************************************************************/
void w_Home::system()
{
    switch (sysStep) {
    case 0:
        sql.ConnectToSql(DB_PATH);
        break;
    case 1:
        server = new tcpServer(this);
        connect(server,SIGNAL(ClientRcvMessage(int,quint64,quint64,QByteArray)),
                this,SLOT(ClientRcvMessage(int,quint64,quint64,QByteArray)));
        connect(server,SIGNAL(ClientDisconnect(int)),
                this,SLOT(ClientDisconnect(int)));
        connect(server,SIGNAL(ClientConnected(int)),this,SLOT(ClientConnected(int)));
    default:
        break;
    }

    sysStep++;

    int i;
    int index;
    if (sysStep%100 == 0) {
        for (i=0; i<server->ClientCount; i++) {
            index = server->ClientIndex[i];
            server->tcpPool[index]->HeartBeat();
        }
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      新的连接
******************************************************************************/
void w_Home::ClientConnected(int index)
{
    qDebug() << server->tcpPool[index]->peerAddress().toString()
             << "上线"
             << QTime::currentTime().toString();

    server->tcpPool[index]->SendMessage(query_mac,serverIndex,0);
    server->tcpPool[index]->SendMessage(query_version,serverIndex,0);
    server->tcpPool[index]->SendMessage(query_num,serverIndex,0);
    updateShow();
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      连接关闭
******************************************************************************/
void w_Home::ClientDisconnect(int index)
{
    qDebug() << server->tcpPool[index]->peerAddress()
             << "下线"
             << QTime::currentTime().toString();

    if (server->tcpPool[index]->Info.isInit) {
        insertRow(server->tcpPool[index]->Info.Number,index,"下线");
        updateShow();
    }
    server->tcpPool[index]->Info.isInit = false;
    server->tcpPool[index]->Info.isFree = true;
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      接收数据
******************************************************************************/
void w_Home::ClientRcvMessage(int index, quint64 type, quint64 target, QByteArray data)
{
    if (target != serverIndex) {
        server->tcpPool[target]->SendMessage(type,index,data);
        return;
    }

    switch (type) {
    case send_num:
        server->tcpPool[index]->Info.Number = data;
        createTable(server->tcpPool[index]->Info.Number);
        insertRow(server->tcpPool[index]->Info.Number,index,"上线");
        server->tcpPool[index]->Info.isInit = true;
        break;
    case send_mac:
        server->tcpPool[index]->Info.Hardware = data;
        break;
    case send_version:
        server->tcpPool[index]->Info.Version = data;
        break;
    case send_date:
        break;
    case send_time:
        break;
    case send_state:
        break;
    case send_config:
        server->tcpPool[index]->Info.Param = data;
        insertRow(server->tcpPool[index]->Info.Number,index,
                  server->tcpPool[index]->Info.Param);
        qDebug()<<"记录配置";
        break;
    case send_heart:
        server->tcpPool[index]->HeartClear();
        return;
    case send_test:
        insertRow(server->tcpPool[index]->Info.Number,index,"测试");
        qDebug()<<"记录测试";
        return;
    case send_error:
        insertRow(server->tcpPool[index]->Info.Number,index,"错误");
        qDebug()<<"记录错误";
        break;
    default:
        break;
    }
    updateShow();
    ui->lineEditData->setText(data);
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.22
  * brief:      更新显示
******************************************************************************/
void w_Home::updateShow()
{
    int i;
    int index;
    int row = server->ClientCount;
    if (row > W_ROW)
        row = W_ROW;

    for(i=0; i<W_ROW*W_COL; i++)
        pItem[i]->setText("");

    for (i=0; i<row; i++) {
        index = server->ClientIndex[i+page*W_ROW];
        pItem[i*W_COL+0]->setText(QString::number(index));
        pItem[i*W_COL+1]->setText(server->tcpPool[index]->Info.Address);
        pItem[i*W_COL+2]->setText(server->tcpPool[index]->Info.Number);
        pItem[i*W_COL+3]->setText(server->tcpPool[index]->Info.Hardware);
        pItem[i*W_COL+4]->setText(server->tcpPool[index]->Info.Time);
        pItem[i*W_COL+5]->setText(server->tcpPool[index]->Info.Version);
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.22
  * brief:      创建新表格
******************************************************************************/
void w_Home::createTable(QString No)
{
    if (sql.IsExist(No))
        return;
    QString str = "create table :table (ID int primary key,";
    str += "IP varchar(20),";
    str += "NO varchar(20),";
    str += "MAC varchar(20),";
    str += "TIME varchar(20),";
    str += "STATE varchar(20),";
    str += "VERSION varchar(20))";
    str.replace(":table",No);

    QSqlQuery query(sql.db);
    query.prepare(str);
    query.exec();
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.22
  * brief:      插入一条记录
******************************************************************************/
void w_Home::insertRow(QString No ,int index, QString state)
{
    QString str = "insert into :table (ID,IP,NO,MAC,TIME,STATE,VERSION)";
    str += "values(:ID,:IP,:NO,:MAC,:TIME,:STATE,:VERSION)";
    str.replace(":table",No);
    QSqlQuery query(sql.db);
    query.prepare(str);
    query.bindValue(":ID",sql.MaxID(No)+1);
    query.bindValue(":IP",server->tcpPool[index]->Info.Address);
    query.bindValue(":NO",server->tcpPool[index]->Info.Number);
    query.bindValue(":MAC",server->tcpPool[index]->Info.Hardware);
    query.bindValue(":TIME",QTime::currentTime().toString());
    query.bindValue(":STATE",state);
    query.bindValue(":VERSION",server->tcpPool[index]->Info.Version);
    query.exec();
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.22
  * brief:      开始监听
******************************************************************************/
void w_Home::on_pushButtonStart_clicked()
{
    if (!isServerOn) {
        isServerOn = true;
        ui->pushButtonStart->setText(tr("停止监听"));
        server->listen(QHostAddress::Any,6000);
    } else {
        isServerOn = false;
        ui->pushButtonStart->setText(tr("开始监听"));
        server->CloseAllClient();
        server->close();
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.22
  * brief:      上一页
******************************************************************************/
void w_Home::on_pushButtonPrev_clicked()
{
    if (page > 0) {
        page--;
        updateShow();
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.22
  * brief:      下一页
******************************************************************************/
void w_Home::on_pushButtonNext_clicked()
{
    if (page < server->ClientCount/W_ROW) {
        page++;
        updateShow();
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.29
  * brief:      测试发送文件
******************************************************************************/
void w_Home::on_pushButtonFile_clicked()
{
    int ret = ui->tableWidget->currentRow();
    if (ret < 0)
        return;

    if (ui->lineEditFile->text().isEmpty())
        return;
    QString fileName = ui->lineEditFile->text();

    int index = server->ClientIndex[ret+page*W_ROW];


    server->tcpPool[index]->StartTransfer(fileName);
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.22
  * brief:      发送命令
******************************************************************************/
void w_Home::on_pushButtonCmd_clicked()
{
    if (ui->lineEditSend->text().isEmpty())
        return;

    int ret = ui->tableWidget->currentRow();
    if (ret < 0)
        return;

    int index = server->ClientIndex[ret+page*W_ROW];

    quint64 type = (quint64)ui->lineEditSend->text().toInt();
    server->tcpPool[index]->SendMessage(type,(quint64)max_client,0);
}
