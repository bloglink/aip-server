#ifndef W_HOME_H
#define W_HOME_H

#include <QWidget>
#include <tcpclient.h>
#include <tcpserver.h>
#include <QByteArray>
#include <QTableWidgetItem>

#include <linux_sql.h>

namespace Ui {
class w_Home;
}

class w_Home : public QWidget
{
    Q_OBJECT
    
public:
    explicit w_Home(QWidget *parent = 0);
    ~w_Home();
    

private:
    Ui::w_Home *ui;

private:
    int page;

    LINUX_SQL sql;

    bool isServerOn;
    tcpServer *server;

    QList<QTableWidgetItem *> pItem;

private slots:
    void updateShow();
    void newRecord(QString No,int state);
    void updateData(QByteArray data);
    void createTable(QString No);
    void insertRow(QString No, int row, QString state);

    void on_pushButton_clicked();

    void on_pushButtonStart_clicked();


};

#endif // W_HOME_H
