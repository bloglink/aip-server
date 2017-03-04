#include "WinServer.h"
#include "ui_WinServer.h"

WinServer::WinServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinServer)
{
    ui->setupUi(this);
    tcpServer = NULL;
    sql = NULL;

    WinInit();
    BtnInit();
    DatInit();
    SqlInit();

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(TcpKeepLive()));
    SockectCount = 0;
}

WinServer::~WinServer()
{
    for (int i=0; i<MaxThreads; i++) {
        Threads.at(i)->quit();
        Threads.at(i)->wait();
    }
    delete ui;
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.02.20
 * brief:       初始化界面和线程池
******************************************************************************/
void WinServer::WinInit()
{
    for (int i=0; i<MaxThreads; i++) {
        Threads.append(new QThread(this));
        Threads.at(i)->start();
    }
    this->setWindowTitle("二代服务器V0.3.0.3");
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.03.24
 * brief:       初始化按键
******************************************************************************/
void WinServer::BtnInit()
{
    connect(ui->TabServer,SIGNAL(clicked(QModelIndex)),this,SLOT(TcpDevSelect(QModelIndex)));
    connect(ui->TabFiles,SIGNAL(clicked(QModelIndex)),this,SLOT(ViewClick(QModelIndex)));
    connect(ui->BtnGetFiles,SIGNAL(clicked(bool)),this,SLOT(GetGuestFiles()));
    connect(ui->BtnGetCurrentFile,SIGNAL(clicked(bool)),this,SLOT(GetGuestFile()));
    connect(ui->BtnPutCurrentFile,SIGNAL(clicked(bool)),this,SLOT(PutLocalFile()));
    connect(ui->BtnStartServer,SIGNAL(clicked(bool)),this,SLOT(TcpInit()));

    connect(ui->BtnCommand,SIGNAL(clicked(bool)),this,SLOT(PutCommand()));
    connect(ui->EditCommand,SIGNAL(editingFinished()),this,SLOT(PutCommand()));
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.03.24
 * brief:       数据初始化
******************************************************************************/
void WinServer::DatInit()
{
    set = new QSettings(GLOBAL_SET,QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("Server");
    ui->EditUserPort->setText(set->value("UserPort","6000").toString());
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.03.24
 * brief:       数据保存
******************************************************************************/
void WinServer::DatSave()
{
    set->setValue("UserPort", ui->EditUserPort->text());
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.03.24
 * brief:       初始化数据库
******************************************************************************/
void WinServer::SqlInit()
{
    QFile file(SQL);
    if (!file.exists()) {
        file.open(QIODevice::ReadWrite);
        file.close();
    }
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(SQL);
    db.open();

    QSqlQuery query(db);
    query.exec("select count(*) from sqlite_master where type='table' and name='NetRecord'");
    if(query.next() && query.value(0).toInt() == 0) {
        query.prepare("create table NetRecord (ID int primary key,IP text,NO text,MAC text,TIME text,STATE text,VERSION text)");
        query.exec();
    }
    query.exec("select count(*) from sqlite_master where type='table' and name='NetState'");
    if(query.next() && query.value(0).toInt() == 0) {
        query.prepare("create table NetState (ID int primary key,PORT text,IP text,NO text,MAC text,TIME text,STATE text,VERSION text)");
        query.exec();
    }

    query.exec("delete from NetState");

    model = new QSqlTableModel(this);
    model->setTable("NetState");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->TabServer->setModel(model);
    model->setFilter("STATE='上线'");
    model->select();
    model->removeColumn(0);
    ui->TabServer->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->TabServer->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
    ui->TabServer->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);

    sql = new SqlServer(this);
    connect(sql,SIGNAL(TransformCmd(QUrl)),this,SLOT(ExcuteCmd(QUrl)));
    sql->Init();
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.03.24
 * brief:       开始监听
******************************************************************************/
void WinServer::TcpInit()
{
    if (tcpServer == NULL) {
        tcpServer = new TcpServer(this);
        connect(tcpServer,SIGNAL(NewGuest(quint16)),this,SLOT(NewGuest(quint16)));
    }
    if (ui->BtnStartServer->text() == tr("开始监听")) {
        tcpServer->listen(QHostAddress::Any,ui->EditUserPort->text().toInt());
        ui->BtnStartServer->setText(tr("停止监听"));
        timer->start(5000);
    } else {
        timer->stop();
        tcpServer->close();
        ui->BtnStartServer->setText(tr("开始监听"));
    }
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.07.16
 * brief:       心跳
******************************************************************************/
void WinServer::TcpKeepLive()
{
    QUrl url;
    url.setUserName(QString::number(ADDR));
    url.setPort(ADDR);
    url.setQuery(QString::number(HEART_BEAT));
    emit TransformCmd(url);
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.12
 * brief:       选择设备
******************************************************************************/
void WinServer::TcpDevSelect(QModelIndex index)
{
    int row = index.row();
    QString port = model->record(row).value("PORT").toString();
    ui->EditClientPort->setText(port);
    CurrentPath.clear();
    ui->TabFiles->clear();
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.12
 * brief:       选择文件
******************************************************************************/
void WinServer::ViewClick(QModelIndex)
{
    QString temp = ui->TabFiles->currentItem()->text(1);
    QString name = ui->TabFiles->currentItem()->text(0);
    if (temp == "文件夹") {
        int t = CurrentPath.lastIndexOf("/");
        if (name == "..") {
            CurrentPath = CurrentPath.mid(0,t);
        } else {
            if (CurrentPath != "/")
                CurrentPath.append("/");
            CurrentPath.append(name);
        }
        GetGuestFiles();
    }
    if (temp == "文件") {
        QString path = CurrentPath;
        path.append("/");
        path.append(name);
        ui->EditClientFile->setText(path);
    }
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.12
 * brief:       获取文件列表
******************************************************************************/
void WinServer::GetGuestFiles()
{
    if (ui->EditClientPort->text().isEmpty())
        return;
    ui->TabFiles->clear();
    if (CurrentPath.isEmpty())
        CurrentPath.append("/");
    int port = ui->EditClientPort->text().toInt();
    QUrl url;
    url.setUserName(QString::number(ADDR));
    url.setPort(port);
    url.setQuery(QString::number(SHELL_CMD));
    url.setFragment(QString("ls -aF %1").arg(CurrentPath));
    isList = true;
    emit TransformCmd(url);
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.12
 * brief:       获取文件
******************************************************************************/
void WinServer::GetGuestFile()
{
    if (ui->EditClientPort->text().isEmpty())
        return;
    if (ui->EditClientFile->text().isEmpty())
        return;
    QString path = ui->EditClientFile->text();
    int port = ui->EditClientPort->text().toInt();
    QUrl url;
    url.setUserName(QString::number(ADDR));
    url.setPort(port);
    url.setQuery(QString::number(GUEST_PUT_HEAD));
    url.setFragment(path);
    emit TransformCmd(url);
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.12
 * brief:       发送文件
******************************************************************************/
void WinServer::PutLocalFile()
{
    if (ui->EditClientPort->text().isEmpty())
        return;
    if (ui->EditLocalFile->text().isEmpty())
        return;
    QString path = ui->EditLocalFile->text();
    int port = ui->EditClientPort->text().toInt();
    QUrl url;
    url.setUserName(QString::number(ADDR));
    url.setPort(port);
    url.setPath(path);
    url.setQuery(QString::number(FILE_HEAD));
    emit TransformCmd(url);
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.12
 * brief:       发送命令
******************************************************************************/
void WinServer::PutCommand()
{
    if (ui->EditCommand->text().isEmpty())
        return;
    if (ui->EditClientPort->text().isEmpty())
        return;
    QUrl url;
    url.setUserName(QString::number(ADDR));
    url.setQuery(QString::number(SHELL_CMD));
    url.setFragment(ui->EditCommand->text());
    url.setPort(ui->EditClientPort->text().toInt());
    isList = false;
    emit TransformCmd(url);
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.12
 * brief:       接收数据
******************************************************************************/
void WinServer::ExcuteCmd(QUrl url)
{
    if (url.port() != ADDR) {
        emit TransformCmd(url);
        return;
    }

    quint16 fun = url.query().toInt();
    switch (fun) {
    case GUEST_LOGIN:
    case ADMIN_LOGIN:
        Login(url);
        model->select();
        break;
    case GUEST_DROPED:
        Droped(url);
        model->select();
        break;
    case ONLINE_DEVICES:
        PutDevices(url);
        break;
    case GUEST_DISPLAY:
        ShowText(url);
        break;
    case SHELL_DAT:
        if (isList)
            ShowFiles(url);
        else
            ShowText(url);
        break;
    default:
        break;
    }
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.18
 * brief:       新的连接
******************************************************************************/
void WinServer::NewGuest(quint16 handle)
{
    TcpSocket *s = new TcpSocket;
    s->setSocketDescriptor(handle);
    s->moveToThread(Threads.at(SockectCount%MaxThreads));
    connect(s,SIGNAL(TransformCmd(QUrl)),this,SLOT(ExcuteCmd(QUrl)));
    connect(this,SIGNAL(TransformCmd(QUrl)),s,SLOT(ExcuteCmd(QUrl)));
    qDebug()<<"new tcp"<<QTime::currentTime().toString();
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.12
 * brief:       用户登录
 * date:        2017.02.11
 * brief:       修正用户登录协议
******************************************************************************/
void WinServer::Login(QUrl url)
{
    sql->InsertState(url);
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.12
 * brief:       用户退出
 * date:        2017.02.11
 * brief:       修正用户退出协议
******************************************************************************/
void WinServer::Droped(QUrl url)
{
    sql->InsertState(url);
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.12
 * brief:       用户显示
******************************************************************************/
void WinServer::ShowText(QUrl url)
{
    QString temp = url.fragment(QUrl::FullyDecoded);
    ui->Text->setText(temp);
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.13
 * brief:       显示文件列表
******************************************************************************/
void WinServer::ShowFiles(QUrl url)
{
    isList = false;
    QString temp = url.fragment(QUrl::FullyDecoded);
    QStringList FileList = temp.split("\n",QString::SkipEmptyParts);
    for (int i=0; i<FileList.size(); i++) {
        QString t = FileList.at(i);
        if (t.endsWith("@") || t.endsWith("|") || t.endsWith("=") || t == "./") {
            continue;
        }
        QTreeWidgetItem *item = new QTreeWidgetItem;
        if (t.endsWith("/")) {
            item->setText(0, t.remove("/"));
            QPixmap pixmap(":/source/dir.png");
            item->setIcon(0, pixmap);
            item->setText(1, "文件夹");
        } else if (t.endsWith("*")) {
            item->setText(0, t.remove("*"));
            QPixmap pixmap(":/source/link.png");
            item->setIcon(0, pixmap);
            item->setText(1, "文件");
        } else {
            item->setText(0, t.remove("*"));
            QPixmap pixmap(":/source/file.png");
            item->setIcon(0, pixmap);
            item->setText(1, "文件");
        }
        ui->TabFiles->addTopLevelItem(item);
    }
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.12
 * brief:       发送在线列表
 * date:        2017.02.11
 * brief:       修正发送在线列表协议
******************************************************************************/
void WinServer::PutDevices(QUrl url)
{
    url.setPort(url.userName().toInt());
    url.setUserName(QString::number(ADDR));
    url.setQuery(QString::number(ONLINE_DEVICES));

    QStringList ItemText;
    for (int i=0; i<model->rowCount(); i++) {
        QStringList s;
        s.append(model->record(i).value("PORT").toString());
        s.append(model->record(i).value("IP").toString());
        s.append(model->record(i).value("NO").toString());
        s.append(model->record(i).value("MAC").toString());
        s.append(model->record(i).value("TIME").toString());
        s.append(model->record(i).value("VERSION").toString());
        ItemText.append(s.join(" "));
    }
    url.setFragment(ItemText.join("@@"));
    emit TransformCmd(url);
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2017.01.18
 * brief:       退出保存
******************************************************************************/
void WinServer::hideEvent(QHideEvent *)
{
    DatSave();
}
/*******************************************************************************
 *                                  END
*******************************************************************************/