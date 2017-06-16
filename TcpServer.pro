#-------------------------------------------------
#
# Project created by QtCreator 2017-02-03T11:13:48
#
#-------------------------------------------------

QT       += core network sql
QT       -= gui

TARGET = TcpServer
TEMPLATE = app


SOURCES += main.cpp \
    WebServer.cpp \
    TcpServer.cpp \
    TcpSocket.cpp \
    SqlServer.cpp

HEADERS  += \
    WebServer.h \
    TcpServer.h \
    TcpSocket.h \
    SqlServer.h

FORMS    +=

RESOURCES +=
