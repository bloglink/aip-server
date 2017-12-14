#ifndef CWINSERVER_H
#define CWINSERVER_H

#include <QFile>
#include <QTimer>
#include <QThread>
#include <QWidget>
#include <QString>
#include <QProcess>
#include <QByteArray>
#include <QStringList>
#include <QButtonGroup>
#include <QTableWidgetItem>
#include <QHostAddress>
#include <QCryptographicHash>

#include "TcpServer.h"
#include "TcpSocket.h"
#include "SqlServer.h"

#define W_ROW 250
#define W_COL 6

namespace Ui {
class CWinServer;
}

class CWinServer : public QWidget
{
    Q_OBJECT
public:
    explicit CWinServer(QWidget *parent = 0);
    ~CWinServer();
private:
    Ui::CWinServer *ui;
signals:
    void TcpClose(void);
    void SendMsg(quint16 port,QString name);
private slots:
    void WinInit(void);
    void KeyInit(void);
    void KeyJudge(int key);
    void DatInit(void);
    void SqlInit(void);
    void TcpInit(void);
    void TcpKeepLive(void);
    void TcpPutBlock(quint16 addr,quint16 cmd,QByteArray msg);
    void TcpLogin(quint16 addr,QByteArray msg);
    void TcpClientInit(QByteArray msg);
    void TcpClientQuit(quint16 addr);
    void TcpPutListMsg(quint16 addr);
    void TcpPutCommandMsg(void);
    void TcpPutFileHeadMsg(void);
    void TcpGetFileHeadMsg(void);
    void ExcuteCmd(quint16 addr,quint16 cmd, QByteArray msg);
    quint16 CurrentPort(void);

private:
    QFile *file;
    QTimer *timer;
    TcpServer *tcp;
    SqlServer *sql;
    QStringList ItemText;



};

#endif // CWINSERVER_H
