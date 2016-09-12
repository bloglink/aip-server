#ifndef CWINSERVER_H
#define CWINSERVER_H

#include <QTimer>
#include <QThread>
#include <QWidget>
#include <QString>
#include <QByteArray>
#include <QStringList>
#include <QButtonGroup>
#include <QTableWidgetItem>

#include "TcpServer.h"

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
    void SeverInit(void);
    void RecvMsg(quint16 port, QByteArray msg);
    void SendMsg(quint16 port, QByteArray msg);
private:
    TcpServer *server;
    QStringList ItemText;
    QList<QTableWidgetItem *> tableItem;
};

#endif // CWINSERVER_H
