#ifndef WINSERVER_H
#define WINSERVER_H

#include <QTimer>
#include <QThread>
#include <QWidget>
#include <QString>
#include <QProcess>
#include <QSettings>
#include <QByteArray>
#include <QStringList>
#include <QButtonGroup>
#include <QTableWidgetItem>
#include <QTreeWidgetItem>
#include <QHostAddress>
#include <QCryptographicHash>
#include <QSqlTableModel>
#include <QSqlRecord>

#include "TcpServer.h"
#include "TcpSocket.h"
#include "SqlServer.h"

const quint8 MaxThreads = 8;

namespace Ui {
class WinServer;
}

class WinServer : public QWidget
{
    Q_OBJECT

public:
    explicit WinServer(QWidget *parent = 0);
    ~WinServer();

private:
    Ui::WinServer *ui;

signals:
    void SendMessage(TcpMap map,QByteArray msg);
private slots:
    void InitWindow(void);
    void InitThread(void);
    void InitButton(void);
    void InitSetting(void);
    void SaveSetting(void);
    void InitSqlite(void);

    void InitServer(void);
    void KeepLive(void);
    void ClickItem(QModelIndex index);
    void ClickView(QModelIndex);
    void GetGuestFiles(void);
    void GetGuestFile(void);
    void PutLocalFile(void);
    void PutCommand(void);
    void ReadMessage(TcpMap map,QByteArray msg);
    void OnlineDevices(TcpMap map);
    void ShowShell(QByteArray msg);
    void ShowFiles(QByteArray msg);
    void NewGuest(quint16 handle);
    virtual void hideEvent(QHideEvent *);
private:
    QTimer *timer;

    SqlServer *sql;
    QSettings *set;
    QString CurrentPath;
    bool isList;

    TcpServer *tcpServer;
    QList<QThread*> Threads;
    quint64 SockectCount;

    QSqlDatabase db;
    QSqlTableModel *model;
};

#endif // WINSERVER_H
