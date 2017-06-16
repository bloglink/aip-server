#include <QCoreApplication>
#include "WebServer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    WebServer w;

    return a.exec();
}
