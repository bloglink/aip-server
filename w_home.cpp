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
    str.append(tr("IP"));
    str.append(tr("出厂编号"));
    str.append(tr("MAC地址"));
    str.append(tr("时间"));
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
        connect(server,SIGNAL(ClientRcvData(int,QByteArray)),
                this,SLOT(ClientRcvData(int,QByteArray)));
        connect(server,SIGNAL(ClientDisconnect(int)),
                this,SLOT(ClientDisconnect(int)));
        connect(server,SIGNAL(ClientConnected(int)),this,SLOT(ClientConnected(int)));
    default:
        break;
    }

    sysStep++;

    int i;
    QByteArray msg;
    msg[0] = type_heart;
    if (sysStep%100 == 0) {
        for (i=0; i<server->ClientCount; i++) {
            server->ClientList[i]->Info.heart++;
            server->SendData(server->ClientID[i],msg);
            if (server->ClientList[i]->Info.heart > 5) {
                server->ClientList[i]->disconnectFromHost();
            }
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
    QByteArray msg;
    msg[0] = type_msg;
    msg.append(QString::number(type_ip));
    server->SendData(index,msg);

    msg.clear();
    msg[0] = type_msg;
    msg.append(QString::number(type_No));
    server->SendData(index,msg);

    msg.clear();
    msg[0] = type_msg;
    msg.append(QString::number(type_mac));
    server->SendData(index,msg);

    msg.clear();
    msg[0] = type_msg;
    msg.append(QString::number(type_version));
    server->SendData(index,msg);
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      连接关闭
******************************************************************************/
void w_Home::ClientDisconnect(int index)
{
    if (server->tcpPool[index]->Info.isInit) {
        newRecord(index, state_lower);
        updateShow();
        server->tcpPool[index]->Info.isInit = false;
        server->tcpPool[index]->Info.isFree = true;
    }
}

/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      接收数据
******************************************************************************/
void w_Home::ClientRcvData(int index, QByteArray data)
{
    quint8 fun = quint8(data[0]);
    data.remove(0,1);
    qDebug()<<data;

    switch (fun) {
    case type_ip:
        server->tcpPool[index]->Info.IP = data;
        break;
    case type_No:
        server->tcpPool[index]->Info.NO = data;
        break;
    case type_mac:
        server->tcpPool[index]->Info.MAC = data;
        break;
    case type_version:
        server->tcpPool[index]->Info.VERSION = data;
        if (server->tcpPool[index]->Info.isInit == false) {
            newRecord(index,state_upper);
            server->tcpPool[index]->Info.isInit = true;
        }
        break;
    case type_test:
        newRecord(index, state_test);
        break;
    case type_config:
        server->tcpPool[index]->Info.PARAM = data;
        newRecord(index, state_config);
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
    int row = server->ClientCount;
    if (row > W_ROW)
        row = W_ROW;

    for(i=0; i<W_ROW*W_COL; i++)
        pItem[i]->setText("");

    for (i=0; i<row; i++) {
        pItem[i*W_COL+0]->setText(server->ClientList[i+page*W_ROW]->Info.ID);
        pItem[i*W_COL+1]->setText(server->ClientList[i+page*W_ROW]->Info.IP);
        pItem[i*W_COL+2]->setText(server->ClientList[i+page*W_ROW]->Info.NO);
        pItem[i*W_COL+3]->setText(server->ClientList[i+page*W_ROW]->Info.MAC);
        pItem[i*W_COL+4]->setText(server->ClientList[i+page*W_ROW]->Info.TIME);
        pItem[i*W_COL+5]->setText(server->ClientList[i+page*W_ROW]->Info.VERSION);
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.22
  * brief:      发送数据
******************************************************************************/
void w_Home::on_pushButton_clicked()
{
    if (ui->lineEditSend->text().isEmpty())
        return;

    QByteArray data;

    data[0] = type_msg;
    data.append(ui->lineEditSend->text().toUtf8());

    int ret = ui->tableWidget->currentRow();
    int index = server->ClientList[ret+page*W_ROW]->Info.ID.toInt();

    if (ret < 0)
        server->SendDataCurrent(data);
    else
        server->SendData(index, data);
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
  * brief:      新记录
******************************************************************************/
void w_Home::newRecord(int index,int state)
{
    createTable(server->tcpPool[index]->Info.NO);

    switch (state) {
    case state_upper:
        if (server->tcpPool[index]->Info.isInit == false) {
            insertRow(server->tcpPool[index]->Info.NO,index,"上线");
            server->tcpPool[index]->Info.isInit = true;
            qDebug()<<"上线";
        }
        break;
    case state_lower:
        if (server->tcpPool[index]->Info.isInit) {
            server->tcpPool[index]->Info.isInit = false;
            server->tcpPool[index]->Info.isFree = true;
            insertRow(server->tcpPool[index]->Info.NO,index,"下线");
            updateShow();
            qDebug()<<"下线";
        }
        break;
    case state_error:
        insertRow(server->tcpPool[index]->Info.NO,index,"错误");
        qDebug()<<"错误";
        break;
    case state_test:
        insertRow(server->tcpPool[index]->Info.NO,index,"测试");
        qDebug()<<"测试";
        break;
    case state_config:
        insertRow(server->tcpPool[index]->Info.NO,index,
                  server->tcpPool[index]->Info.PARAM);
        qDebug()<<"更改配置";
        break;
    default:
        break;
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
    query.bindValue(":IP",server->tcpPool[index]->Info.IP);
    query.bindValue(":NO",server->tcpPool[index]->Info.NO);
    query.bindValue(":MAC",server->tcpPool[index]->Info.MAC);
    query.bindValue(":TIME",QTime::currentTime().toString());
    query.bindValue(":STATE",state);
    query.bindValue(":VERSION",server->tcpPool[index]->Info.VERSION);
    query.exec();
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
