#-------------------------------------------------
#
# Project created by QtCreator 2017-02-03T11:13:48
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = aip-server
TEMPLATE = app


SOURCES += main.cpp\
    SqlServer.cpp \
    TcpServer.cpp \
    TcpSocket.cpp \
    WebServer.cpp

HEADERS  += \
    SqlServer.h \
    TcpServer.h \
    TcpSocket.h \
    WebServer.h

FORMS    += \
    WebServer.ui

RESOURCES += \
    image.qrc
