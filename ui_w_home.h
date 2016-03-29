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
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_w_Home
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QTableWidget *tableWidget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *lineEditData;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEditFile;
    QPushButton *pushButtonFile;
    QLabel *label_3;
    QLineEdit *lineEditSend;
    QPushButton *pushButtonCmd;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pushButtonStart;
    QPushButton *pushButtonPrev;
    QPushButton *pushButtonNext;

    void setupUi(QWidget *w_Home)
    {
        if (w_Home->objectName().isEmpty())
            w_Home->setObjectName(QString::fromUtf8("w_Home"));
        w_Home->resize(795, 597);
        gridLayout_2 = new QGridLayout(w_Home);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tableWidget = new QTableWidget(w_Home);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget->horizontalHeader()->setHighlightSections(false);
        tableWidget->verticalHeader()->setHighlightSections(false);

        gridLayout->addWidget(tableWidget, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(20);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(w_Home);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        lineEditData = new QLineEdit(w_Home);
        lineEditData->setObjectName(QString::fromUtf8("lineEditData"));

        horizontalLayout_2->addWidget(lineEditData);


        gridLayout_2->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(20);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(w_Home);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        lineEditFile = new QLineEdit(w_Home);
        lineEditFile->setObjectName(QString::fromUtf8("lineEditFile"));

        horizontalLayout->addWidget(lineEditFile);

        pushButtonFile = new QPushButton(w_Home);
        pushButtonFile->setObjectName(QString::fromUtf8("pushButtonFile"));

        horizontalLayout->addWidget(pushButtonFile);

        label_3 = new QLabel(w_Home);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout->addWidget(label_3);

        lineEditSend = new QLineEdit(w_Home);
        lineEditSend->setObjectName(QString::fromUtf8("lineEditSend"));

        horizontalLayout->addWidget(lineEditSend);

        pushButtonCmd = new QPushButton(w_Home);
        pushButtonCmd->setObjectName(QString::fromUtf8("pushButtonCmd"));

        horizontalLayout->addWidget(pushButtonCmd);


        gridLayout_2->addLayout(horizontalLayout, 2, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(20);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        pushButtonStart = new QPushButton(w_Home);
        pushButtonStart->setObjectName(QString::fromUtf8("pushButtonStart"));

        horizontalLayout_4->addWidget(pushButtonStart);

        pushButtonPrev = new QPushButton(w_Home);
        pushButtonPrev->setObjectName(QString::fromUtf8("pushButtonPrev"));

        horizontalLayout_4->addWidget(pushButtonPrev);

        pushButtonNext = new QPushButton(w_Home);
        pushButtonNext->setObjectName(QString::fromUtf8("pushButtonNext"));

        horizontalLayout_4->addWidget(pushButtonNext);


        gridLayout_2->addLayout(horizontalLayout_4, 3, 0, 1, 1);


        retranslateUi(w_Home);

        QMetaObject::connectSlotsByName(w_Home);
    } // setupUi

    void retranslateUi(QWidget *w_Home)
    {
        w_Home->setWindowTitle(QApplication::translate("w_Home", "\351\235\222\345\262\233\350\211\276\346\231\256\346\231\272\350\203\275\344\273\252\345\231\250TCP\346\234\215\345\212\241\345\267\245\345\205\267--by\350\265\265\346\245\240\346\236\227", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("w_Home", "\346\216\245\346\224\266\346\225\260\346\215\256", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("w_Home", "\345\217\221\351\200\201\346\226\207\344\273\266", 0, QApplication::UnicodeUTF8));
        pushButtonFile->setText(QApplication::translate("w_Home", "\345\217\221\351\200\201\346\226\207\344\273\266", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("w_Home", "\345\217\221\351\200\201\345\221\275\344\273\244", 0, QApplication::UnicodeUTF8));
        pushButtonCmd->setText(QApplication::translate("w_Home", "\345\217\221\351\200\201\345\221\275\344\273\244", 0, QApplication::UnicodeUTF8));
        pushButtonStart->setText(QApplication::translate("w_Home", "\345\274\200\345\247\213\347\233\221\345\220\254", 0, QApplication::UnicodeUTF8));
        pushButtonPrev->setText(QApplication::translate("w_Home", "\344\270\212\344\270\200\351\241\265", 0, QApplication::UnicodeUTF8));
        pushButtonNext->setText(QApplication::translate("w_Home", "\344\270\213\344\270\200\351\241\265", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class w_Home: public Ui_w_Home {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_W_HOME_H
