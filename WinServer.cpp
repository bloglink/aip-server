#include "WinServer.h"
#include "ui_WinServer.h"

WinServer::WinServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinServer)
{
    ui->setupUi(this);
    tcpServer = NULL;
    sql = NULL;

    InitWindow();
    InitButton();
    InitSetting();
    InitSqlite();
    InitThread();

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(KeepLive()));
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
void WinServer::InitWindow()
{
    this->setWindowTitle("二代服务器V-0.3.0.5");
    qRegisterMetaType<TcpMap>("TcpMap");
}

void WinServer::InitThread()
{
    for (int i=0; i<MaxThreads; i++) {
        Threads.append(new QThread(this));
        Threads.at(i)->start();
    }
}
void WinServer::InitButton()
{
    connect(ui->TabServer,SIGNAL(clicked(QModelIndex)),this,SLOT(ClickItem(QModelIndex)));
    connect(ui->TabFiles,SIGNAL(clicked(QModelIndex)),this,SLOT(ClickView(QModelIndex)));
    connect(ui->BtnGetFiles,SIGNAL(clicked(bool)),this,SLOT(GetGuestFiles()));
    connect(ui->BtnGetCurrentFile,SIGNAL(clicked(bool)),this,SLOT(GetGuestFile()));
    connect(ui->BtnPutCurrentFile,SIGNAL(clicked(bool)),this,SLOT(PutLocalFile()));
    connect(ui->BtnStartServer,SIGNAL(clicked(bool)),this,SLOT(InitServer()));

    connect(ui->BtnCommand,SIGNAL(clicked(bool)),this,SLOT(PutCommand()));
    connect(ui->EditCommand,SIGNAL(editingFinished()),this,SLOT(PutCommand()));
}

void WinServer::InitSetting()
{
    set = new QSettings(GLOBAL_SET,QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("Server");
    ui->EditUserPort->setText(set->value("UserPort","6000").toString());
}

void WinServer::SaveSetting()
{
    set->setValue("UserPort", ui->EditUserPort->text());
}

void WinServer::InitSqlite()
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
    connect(this,SIGNAL(SendMessage(TcpMap,QByteArray)),sql,
            SLOT(ReadMessage(TcpMap,QByteArray)));
    connect(sql,SIGNAL(SendMessage(TcpMap,QByteArray)),this,
            SLOT(ReadMessage(TcpMap,QByteArray)));
    sql->Init();
}

void WinServer::InitServer()
{
    if (tcpServer == NULL) {
        tcpServer = new TcpServer(this);
        connect(tcpServer,SIGNAL(NewGuest(quint16)),this,SLOT(NewGuest(quint16)));
    }
    if (ui->BtnStartServer->text() == tr("开始监听")) {
        tcpServer->listen(QHostAddress::AnyIPv4,ui->EditUserPort->text().toInt());
        ui->BtnStartServer->setText(tr("停止监听"));
        timer->start(5000);
    } else {
        timer->stop();
        tcpServer->close();
        ui->BtnStartServer->setText(tr("开始监听"));
    }
}

void WinServer::KeepLive()
{
    TcpMap map;
    map.insert("TxAddress",ADDR);
    map.insert("RxAddress",ADDR);
    map.insert("TxCommand",HEART_BEAT);
    emit SendMessage(map,NULL);
}

void WinServer::ClickItem(QModelIndex index)
{
    int row = index.row();
    QString port = model->record(row).value("PORT").toString();
    ui->EditClientPort->setText(port);
    CurrentPath.clear();
    ui->TabFiles->clear();
}

void WinServer::ClickView(QModelIndex)
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

void WinServer::GetGuestFiles()
{
    if (ui->EditClientPort->text().isEmpty())
        return;
    ui->TabFiles->clear();
    if (CurrentPath.isEmpty())
        CurrentPath.append("/");
    int port = ui->EditClientPort->text().toInt();

    TcpMap map;
    map.insert("TxAddress",port);
    map.insert("RxAddress",ADDR);
    map.insert("TxCommand",SHELL_CMD);
    isList = true;
    emit SendMessage(map,QString("ls -aF %1").arg(CurrentPath).toUtf8());
}

void WinServer::GetGuestFile()
{
    if (ui->EditClientPort->text().isEmpty())
        return;
    if (ui->EditClientFile->text().isEmpty())
        return;
    QString path = ui->EditClientFile->text();
    int port = ui->EditClientPort->text().toInt();

    TcpMap map;
    map.insert("TxAddress",port);
    map.insert("RxAddress",ADDR);
    map.insert("TxCommand",GUEST_PUT_HEAD);
    emit SendMessage(map,path.toUtf8());
}

void WinServer::PutLocalFile()
{
    if (ui->EditClientPort->text().isEmpty())
        return;
    if (ui->EditLocalFile->text().isEmpty())
        return;
    QString path = ui->EditLocalFile->text();
    int port = ui->EditClientPort->text().toInt();

    TcpMap map;
    map.insert("TxAddress",port);
    map.insert("RxAddress",ADDR);
    map.insert("TxCommand",FILE_HEAD);
    emit SendMessage(map,path.toUtf8());
}

void WinServer::PutCommand()
{
    if (ui->EditCommand->text().isEmpty())
        return;
    if (ui->EditClientPort->text().isEmpty())
        return;

    TcpMap map;
    map.insert("TxAddress",ui->EditClientPort->text().toInt());
    map.insert("RxAddress",ADDR);
    map.insert("TxCommand",SHELL_CMD);
    emit SendMessage(map,ui->EditCommand->text().toUtf8());
    isList = false;
}
void WinServer::ReadMessage(TcpMap map, QByteArray msg)
{
    quint16 TxAddress = map.value("TxAddress");
    quint16 TxCommand = map.value("TxCommand");
    if (TxAddress!=ADDR) {
        emit SendMessage(map,msg);
        return;
    }
    switch (TxCommand) {
    case GUEST_LOGIN:
    case ADMIN_LOGIN:
        qDebug()<<QTime::currentTime().toString()<<"登录";
        sql->ReadMessage(map,msg);
        model->select();
        qDebug()<<QTime::currentTime().toString()<<"登录OK";
        break;
    case GUEST_DROPED:
        qDebug()<<QTime::currentTime().toString()<<"登出";
        sql->ReadMessage(map,msg);
        model->select();
        qDebug()<<QTime::currentTime().toString()<<"登出OK";
        break;
    case ONLINE_DEVICES:
        OnlineDevices(map);
        break;
    case GUEST_DISPLAY:
        ShowShell(msg);
        break;
    case SHELL_DAT:
        if (isList)
            ShowFiles(msg);
        else
            ShowShell(msg);
        break;
    default:
        break;
    }
}

void WinServer::NewGuest(quint16 handle)
{
    TcpSocket *s = new TcpSocket;
    s->setSocketDescriptor(handle);
    s->moveToThread(Threads.at(SockectCount%MaxThreads));
    connect(this,SIGNAL(SendMessage(TcpMap,QByteArray)),s,
            SLOT(ReadMessage(TcpMap,QByteArray)));
    connect(s,SIGNAL(SendMessage(TcpMap,QByteArray)),this,
            SLOT(ReadMessage(TcpMap,QByteArray)));
    qDebug()<<QTime::currentTime().toString()<<"new tcp connection";
}

void WinServer::OnlineDevices(TcpMap map)
{
    quint16 TxAddress = map.value("TxAddress");
    quint16 RxAddress = map.value("RxAddress");
    QStringList ItemText;
    for (int i=0; i<model->rowCount(); i++) {
        QStringList s;
        s.append(model->record(i).value("PORT").toString());
        s.append(model->record(i).value("IP").toString());
        s.append(model->record(i).value("TIME").toString());
        s.append(model->record(i).value("NO").toString());
        s.append(model->record(i).value("MAC").toString());
        s.append(model->record(i).value("VERSION").toString());
        ItemText.append(s.join(" "));
    }
    map["TxAddress"] = RxAddress;
    map["RxAddress"] = TxAddress;

    emit SendMessage(map,ItemText.join("@@").toUtf8());
}

void WinServer::ShowShell(QByteArray msg)
{
    ui->Text->setText(msg);
}

void WinServer::ShowFiles(QByteArray msg)
{
    isList = false;
    QString temp = msg;
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

void WinServer::hideEvent(QHideEvent *)
{
    SaveSetting();
}
/*********************************END OF FILE**********************************/
