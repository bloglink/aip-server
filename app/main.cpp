/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       服务器
*******************************************************************************/
#include <QCoreApplication>
#include "webserver.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    WebServer w;
    w.initServer();

    return a.exec();
}

