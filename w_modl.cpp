/*******************************************************************************
 * Copyright (c) 2016,青岛艾普智能仪器有限公司
 * All rights reserved.
 *
 * version:     1.0
 * author:      link
 * date:        2016.01.29
 * brief:       型号管理界面
*******************************************************************************/
#include "w_modl.h"

w_Modl::w_Modl(QWidget *parent) :
    QDialog(parent)
{
    table = NULL;

    createButton();
    createTab();

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addLayout(tableLayout);
    layout->addWidget(new QLabel(this));
    layout->addLayout(buttonLayout);

    this->setLayout(layout);
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.01.28
  * brief:      创建button
******************************************************************************/
void w_Modl::createButton()
{
    QStringList buttonName;
    buttonName.append(tr("本天"));
    buttonName.append(tr("七天"));
    buttonName.append(tr("本月"));
    buttonName.append(tr("全部"));
    buttonName.append(tr("清空"));

    QStringList buttonIcon;
    buttonIcon.append(":/image/101.png");
    buttonIcon.append(":/image/102.png");
    buttonIcon.append(":/image/103.png");
    buttonIcon.append(":/image/104.png");
    buttonIcon.append(":/image/105.png");

    QStringList buttonId;
    buttonId.append(QString::number(W_MODL_EXIT));
    buttonId.append(QString::number(W_MODL_IMPORT));
    buttonId.append(QString::number(W_MODL_EXPORT));
    buttonId.append(QString::number(W_MODL_ADD));
    buttonId.append(QString::number(W_MODL_DEL));


    buttonLayout = new QVBoxLayout;
    buttonGroup = new QButtonGroup;

    for (int i=0; i<buttonName.size(); i++){
        buttonList.append(new QToolButton(this));
        buttonList[i]->setText(buttonName[i]);
        buttonList[i]->setIconSize(QSize(ICON,ICON));
        buttonList[i]->setIcon(QIcon(buttonIcon[i]));
        buttonList[i]->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        buttonList[i]->setStyleSheet("QToolButton{border:0px;}QToolButton:hover{color:rgb(255,0,0);}");

        buttonLayout->addWidget(buttonList[i]);

        buttonGroup->addButton(buttonList[i],buttonId[i].toInt());
    }
    buttonLayout->addStretch();

    connect(buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(buttonJudge(int)));
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.01.28
  * brief:      创建tableWidget
******************************************************************************/
void w_Modl::createTab()
{
    QStringList str;
    str.append(tr("ID号"));
    str.append(tr("IP地址"));
    str.append(tr("端口号"));
    str.append(tr("时间"));
    str.append(tr("状态"));

    table = new QTableWidget(this);
    table->setColumnCount(str.size());
    table->setHorizontalHeaderLabels(str);
    table->horizontalHeader()->setHighlightSections(false);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    fileList.append(new QLineEdit(this));

    tableLayout = new QVBoxLayout;
    tableLayout->addWidget(table);
    tableLayout->addWidget(fileList[0]);
}

/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2015.12.10
  * brief:      按钮功能
******************************************************************************/
void w_Modl::buttonJudge(int id)
{
    switch (id){
    case W_MODL_EXIT:
        emit switchWindow(W_HOME);
        break;
    case W_MODL_IMPORT:
        break;
    case W_MODL_EXPORT:
        break;
    default:
        break;
    }
}

/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.01.28
  * brief:      更新显示
******************************************************************************/
void w_Modl::updateShow()
{
    int i;

    table->setRowCount(ip.size());
    for(i=0; i<id.size(); i++)
    {
        QTableWidgetItem *pItem = new QTableWidgetItem(id[i]);
        pItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(i,0,pItem);
    }
    for(i=0; i<ip.size(); i++)
    {
        QTableWidgetItem *pItem = new QTableWidgetItem(ip[i]);
        pItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(i,1,pItem);
    }
    for(i=0; i<port.size(); i++)
    {
        QTableWidgetItem *pItem = new QTableWidgetItem(port[i]);
        pItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(i,2,pItem);
    }

    for(i=0; i<time.size(); i++)
    {
        QTableWidgetItem *pItem = new QTableWidgetItem(time[i]);
        pItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(i,3,pItem);
    }
    for(i=0; i<status.size(); i++)
    {
        QTableWidgetItem *pItem = new QTableWidgetItem(status[i]);
        pItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(i,4,pItem);
    }
}


