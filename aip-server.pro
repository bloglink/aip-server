#-------------------------------------------------
#
# Project created by QtCreator 2016-03-16T11:13:46
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = aip-server
TEMPLATE = app


SOURCES += main.cpp\
        w_back.cpp \
    w_modl.cpp \
    tcpserver.cpp \
    tcpclient.cpp

HEADERS  += w_back.h \
    w_modl.h \
    my_define.h \
    tcpserver.h \
    tcpclient.h

RESOURCES += \
    image.qrc
