#-------------------------------------------------
#
# Project created by QtCreator 2016-03-16T11:13:46
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = aip-server
TEMPLATE = app


SOURCES +=\
    CWinServer.cpp \
    TcpServer.cpp \
    TcpSocket.cpp \
    SqlServer.cpp \
    aip-server.cpp

HEADERS  += \
    CWinServer.h \
    TcpServer.h \
    TcpSocket.h \
    SqlServer.h

RESOURCES += \
    aip-server.qrc

FORMS += \
    CWinServer.ui
