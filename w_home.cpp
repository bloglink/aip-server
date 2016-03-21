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
void w_Home::updateData(QByteArray data)
{
    ui->lineEditData->setText(data);
}

void w_Home::updateShow()
{
    int i;
    int row = server->ClientCount();
    if (row > W_ROW)
        row = W_ROW;

    for(i=0; i<W_ROW*W_COL; i++)
        pItem[i]->setText("");

    for (i=0; i<row; i++) {
        pItem[i+0]->setText(server->ClientList[i+page*W_ROW]->Info.Id);
        pItem[i+1]->setText(server->ClientList[i+page*W_ROW]->Info.Ip);
        pItem[i+2]->setText(server->ClientList[i+page*W_ROW]->Info.No);
        pItem[i+3]->setText(server->ClientList[i+page*W_ROW]->Info.Mac);
        pItem[i+4]->setText(server->ClientList[i+page*W_ROW]->Info.Port);
        pItem[i+5]->setText(server->ClientList[i+page*W_ROW]->Info.Time);
        pItem[i+6]->setText(server->ClientList[i+page*W_ROW]->Info.Version);
    }
}

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
void w_Home::newRecord(QString No,int state)
{
    switch (state) {
    case state_upper:
        if (sql.isExist(No))
            qDebug()<<sql.selectMax(No);
        qDebug()<<"上线";
        break;
    case state_lower:
        qDebug()<<"下线";
        break;
    case state_error:
        qDebug()<<"错误";
        break;
    case state_test:
        qDebug()<<"测试";
        break;
    default:
        break;
    }
}
