#include <QApplication>
#include <QTextCodec>
#include <w_home.h>
int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    QApplication a(argc, argv);
    w_Home w;
    w.showMaximized();
    w.show();
    
    return a.exec();
}
