#include "w_home.h"
#include "ui_w_home.h"

w_Home::w_Home(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::w_Home)
{
    int i;
    ui->setupUi(this);

    isServerOn = false;
    page = 0;

    QStringList str;
    str.append(tr("ID"));
    str.append(tr("IP"));
    str.append(tr("出厂编号"));
    str.append(tr("MAC地址"));
    str.append(tr("端口号"));
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

    sql.createConnetion(DB_PATH);

    server = new tcpServer(this);

    connect(server,SIGNAL(shareData(QByteArray)),
            this,SLOT(updateData(QByteArray)));
    connect(server,SIGNAL(updateShow()),this,SLOT(updateShow()));
    connect(server,SIGNAL(newRecord(QString,int)),
            this,SLOT(newRecord(QString,int)));
}

w_Home::~w_Home()
{
    sql.close_connection();
    delete ui;
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.22
  * brief:      显示数据
******************************************************************************/
void w_Home::updateData(QByteArray data)
{
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
    int row = server->ClientCount();
    if (row > W_ROW)
        row = W_ROW;

    for(i=0; i<W_ROW*W_COL; i++)
        pItem[i]->setText("");

    for (i=0; i<row; i++) {
        pItem[i*W_COL+0]->setText(server->ClientList[i+page*W_ROW]->Info.Id);
        pItem[i*W_COL+1]->setText(server->ClientList[i+page*W_ROW]->Info.Ip);
        pItem[i*W_COL+2]->setText(server->ClientList[i+page*W_ROW]->Info.No);
        pItem[i*W_COL+3]->setText(server->ClientList[i+page*W_ROW]->Info.Mac);
        pItem[i*W_COL+4]->setText(server->ClientList[i+page*W_ROW]->Info.Port);
        pItem[i*W_COL+5]->setText(server->ClientList[i+page*W_ROW]->Info.Time);
        pItem[i*W_COL+6]->setText(server->ClientList[i+page*W_ROW]->Info.Version);
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
    QByteArray data;

    data[0] = sendtype_msg;
    data.append(ui->lineEditSend->text().toUtf8());

    int ret = ui->tableWidget->currentRow();
    int clientID = server->ClientList[ret+page*W_ROW]->Info.Id.toInt();

    if (ui->lineEditSend->text().isEmpty())
        return;

    if (ret < 0)
        server->SendDataCurrent(data);
    else
        server->SendData(clientID, data);
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
void w_Home::newRecord(QString No,int state)
{
    int i;

    createTable(No);

    switch (state) {
    case state_upper:
        for (i=0; i<server->ClientCount(); i++) {
            if (server->ClientList[i]->Info.No == No)
                insertRow(No,i,"上线");
        }
        qDebug()<<"上线";
        break;
    case state_lower:
        for (i=0; i<server->ClientCount(); i++) {
            if (server->ClientList[i]->Info.No == No)
                insertRow(No,i,"下线");
        }
        qDebug()<<"下线";
        break;
    case state_error:
        for (i=0; i<server->ClientCount(); i++) {
            if (server->ClientList[i]->Info.No == No)
                insertRow(No,i,"错误");
        }
        qDebug()<<"错误";
        break;
    case state_test:
        for (i=0; i<server->ClientCount(); i++) {
            if (server->ClientList[i]->Info.No == No)
                insertRow(No,i,"测试");
        }
        qDebug()<<"测试";
        break;
    case state_config:
        for (i=0; i<server->ClientCount(); i++) {
            if (server->ClientList[i]->Info.No == No)
                insertRow(No,i,"更改");
        }
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
    if (sql.isExist(No))
        return;
    QString str = "create table :table (ID int primary key,IP varchar(20),NO varchar(20),MAC varchar(20),PORT varchar(20),TIME varchar(20),STATE varchar(20))";
    //    str += "IP varchar(20),";
    //    str += "NO varchar(20),";
    //    str += "MAC varchar(20),";
    //    str += "PORT varchar(20),";
    //    str += "TIME varchar(20),";
    //    str += "STATE varchar(20))";
    str.replace(":table",No);

    QSqlQuery query(sql.db);
    query.prepare(str);
    bool ok = query.exec();
    qDebug()<<ok;
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.22
  * brief:      插入一条记录
******************************************************************************/
void w_Home::insertRow(QString No ,int row, QString state)
{
    QString str = "insert into :table (ID,IP,NO,MAC,PORT,TIME,STATE)""values(:ID,:IP,:NO,:MAC,:PORT,:TIME,:STATE)";
    str.replace(":table",No);
    QSqlQuery query(sql.db);
    query.prepare(str);
    query.bindValue(":ID",sql.selectMax(No)+1);
    query.bindValue(":IP",server->ClientList[row]->Info.Ip);
    query.bindValue(":NO",server->ClientList[row]->Info.No);
    query.bindValue(":MAC",server->ClientList[row]->Info.Mac);
    query.bindValue(":PORT",server->ClientList[row]->Info.Port);
    query.bindValue(":TIME",QTime::currentTime().toString());
    query.bindValue(":STATE",state);
    query.exec();
}
