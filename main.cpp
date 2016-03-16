#include "w_back.h"
#include <QApplication>
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    QApplication a(argc, argv);
    w_Back w;
    w.showMaximized();
    w.show();
    
    return a.exec();
}
