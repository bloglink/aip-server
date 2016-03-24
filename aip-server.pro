#-------------------------------------------------
#
# Project created by QtCreator 2016-03-16T11:13:46
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = aip-server
TEMPLATE = app


SOURCES += main.cpp\
    tcpserver.cpp \
    tcpclient.cpp \
    w_home.cpp \
    linux_sql.cpp

HEADERS  += \
    my_define.h \
    tcpserver.h \
    tcpclient.h \
    w_home.h \
    linux_sql.h

RESOURCES +=

FORMS += \
    w_home.ui
