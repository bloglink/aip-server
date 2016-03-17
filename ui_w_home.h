/********************************************************************************
** Form generated from reading UI file 'w_home.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_W_HOME_H
#define UI_W_HOME_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_w_Home
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QTableWidget *tableWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLineEdit *lineEditData;
    QLabel *label_3;
    QLineEdit *lineEditSend;
    QPushButton *pushButton;
    QPushButton *pushButtonStart;

    void setupUi(QWidget *w_Home)
    {
        if (w_Home->objectName().isEmpty())
            w_Home->setObjectName(QString::fromUtf8("w_Home"));
        w_Home->resize(795, 597);
        verticalLayout = new QVBoxLayout(w_Home);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tableWidget = new QTableWidget(w_Home);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget->horizontalHeader()->setHighlightSections(false);
        tableWidget->verticalHeader()->setHighlightSections(false);

        gridLayout->addWidget(tableWidget, 0, 0, 1, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_2 = new QLabel(w_Home);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        lineEditData = new QLineEdit(w_Home);
        lineEditData->setObjectName(QString::fromUtf8("lineEditData"));

        horizontalLayout->addWidget(lineEditData);

        label_3 = new QLabel(w_Home);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout->addWidget(label_3);

        lineEditSend = new QLineEdit(w_Home);
        lineEditSend->setObjectName(QString::fromUtf8("lineEditSend"));

        horizontalLayout->addWidget(lineEditSend);

        pushButton = new QPushButton(w_Home);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout);

        pushButtonStart = new QPushButton(w_Home);
        pushButtonStart->setObjectName(QString::fromUtf8("pushButtonStart"));

        verticalLayout->addWidget(pushButtonStart);


        retranslateUi(w_Home);

        QMetaObject::connectSlotsByName(w_Home);
    } // setupUi

    void retranslateUi(QWidget *w_Home)
    {
        w_Home->setWindowTitle(QApplication::translate("w_Home", "\351\235\222\345\262\233\350\211\276\346\231\256\346\231\272\350\203\275\344\273\252\345\231\250TCP\346\234\215\345\212\241\345\267\245\345\205\267--by\350\265\265\346\245\240\346\236\227", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("w_Home", "\346\216\245\346\224\266\346\225\260\346\215\256", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("w_Home", "\345\217\221\351\200\201\346\225\260\346\215\256", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("w_Home", "\345\217\221\351\200\201", 0, QApplication::UnicodeUTF8));
        pushButtonStart->setText(QApplication::translate("w_Home", "\345\274\200\345\247\213\347\233\221\345\220\254", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class w_Home: public Ui_w_Home {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_W_HOME_H
