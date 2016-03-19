#include "w_home.h"
#include "ui_w_home.h"

w_Home::w_Home(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::w_Home)
{
    ui->setupUi(this);

    isServerOn = false;

    QStringList str;
    str.append(tr("ID"));
    str.append(tr("IP"));
    str.append(tr("端口号"));
    str.append(tr("时间"));
    str.append(tr("软件版本"));
    str.append(tr("出厂编号"));
    str.append(tr("MAC地址"));

    ui->tableWidget->setColumnCount(str.size());
    ui->tableWidget->setHorizontalHeaderLabels(str);
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    server = new tcpServer(this);

    connect(server,SIGNAL(ClientConnect(int,QString,int)),
            this,SLOT(ClientConnect(int,QString,int)));
    connect(server,SIGNAL(ClientDisConnect(int,QString,int)),
            this,SLOT(ClientDisConnect(int,QString,int)));
    connect(server,SIGNAL(ClientReadData(int,QString,int,QByteArray)),
            this,SLOT(ClientReadData(int,QString,int,QByteArray)));
    connect(server,SIGNAL(updateShow()),this,SLOT(updateShow()));
}

w_Home::~w_Home()
{
    delete ui;
}

void w_Home::ClientReadData(int clientID,QString IP,int Port,QByteArray data)
{
    QString str;
    server->clearHeart(clientID);

    if (!data.isEmpty()){
        str.append(QString::number(clientID));
        str.append(IP);
        str.append(QString::number(Port));
        str.append(data);
        ui->lineEditData->setText(data);
    }
}

void w_Home::ClientConnect(int clientID,QString IP,int Port)
{
//    int i;
//    for (i=0; i<ip.size(); i++) {
//        if (ip[i] == IP)
//            break;
//    }
//    if (i == ip.size()) {
//        id.append(QString::number(clientID));
//        ip.append(IP);
//        port.append(QString::number(Port));
//        time.append(QTime::currentTime().toString());
//        status.append(tr("上线"));
//    } else {
//        id[i] = QString::number(clientID);
//        port[i] = QString::number(Port);
//        time[i] = QTime::currentTime().toString();
//        status[i] = tr("上线");
//    }

//    updateShow();
}

void w_Home::ClientDisConnect(int,QString IP,int)
{
    int i;
    for (i=0; i<ip.size(); i++) {
        if (ip[i] == IP) {
            id.removeAt(i);
            ip.removeAt(i);
            port.removeAt(i);
            time.removeAt(i);
            status.removeAt(i);
            break;
        }
    }
    updateShow();
}
void w_Home::updateShow()
{
    int i;

    id.clear();
    ip.clear();
    No.clear();
    Mac.clear();
    port.clear();
    time.clear();
    version.clear();
    for (i=0; i<server->ClientCount(); i++) {
        id.append(server->ClientList[i]->Info.Id);
        ip.append(server->ClientList[i]->Info.Ip);
        No.append(server->ClientList[i]->Info.No);
        Mac.append(server->ClientList[i]->Info.Mac);
        port.append(server->ClientList[i]->Info.Port);
        time.append(server->ClientList[i]->Info.Time);
        version.append(server->ClientList[i]->Info.Version);
    }

    ui->tableWidget->setRowCount(ip.size());

    for(i=0; i<id.size(); i++)
    {
        QTableWidgetItem *pItem = new QTableWidgetItem(id[i]);
        pItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,0,pItem);
    }
    for(i=0; i<ip.size(); i++)
    {
        QTableWidgetItem *pItem = new QTableWidgetItem(ip[i]);
        pItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,1,pItem);
    }
    for(i=0; i<port.size(); i++)
    {
        QTableWidgetItem *pItem = new QTableWidgetItem(port[i]);
        pItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,2,pItem);
    }

    for(i=0; i<time.size(); i++)
    {
        QTableWidgetItem *pItem = new QTableWidgetItem(time[i]);
        pItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,3,pItem);
    }
    for(i=0; i<version.size(); i++)
    {
        QTableWidgetItem *pItem = new QTableWidgetItem(version[i]);
        pItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,4,pItem);
    }
    for(i=0; i<No.size(); i++)
    {
        QTableWidgetItem *pItem = new QTableWidgetItem(No[i]);
        pItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,5,pItem);
    }
    for(i=0; i<Mac.size(); i++)
    {
        QTableWidgetItem *pItem = new QTableWidgetItem(Mac[i]);
        pItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,6,pItem);
    }
}

void w_Home::on_pushButton_clicked()
{
    int ret = ui->tableWidget->currentRow();

    if (ui->lineEditSend->text().isEmpty())
        return;

    if (ret < 0)
        server->SendDataCurrent(ui->lineEditSend->text().toUtf8());
    else
        server->SendData(id[ret].toInt(),ui->lineEditSend->text().toUtf8());
}

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
