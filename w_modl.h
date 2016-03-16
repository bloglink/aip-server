#ifndef W_MODL_H
#define W_MODL_H

#include <QDialog>

#include <my_define.h>

#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QHeaderView>
#include <QToolButton>
#include <QButtonGroup>
#include <QTableWidget>

class w_Modl : public QDialog
{
    Q_OBJECT
    
public:
    explicit w_Modl(QWidget *parent = 0);

private:
    QList<QToolButton *> buttonList;
    QButtonGroup *buttonGroup;
    QVBoxLayout *buttonLayout;
    QVBoxLayout *tableLayout;

    QList<QLineEdit *> fileList;
    QTableWidget *table;

private slots:
    void createButton();
    void createTab();
    void buttonJudge(int);
public:
    QStringList id;
    QStringList ip;
    QStringList port;
    QStringList time;
    QStringList status;
public slots:
    void updateShow();
signals:
    void switchWindow(int win);
    void exportToCsv(int row);
    void changeFile(int win);
};

#endif // W_MODL_H
