QT += core network sql
QT -= gui

TARGET = aip-server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += \
    app/lqsnowflake.h \
    app/tcpserver.h \
    app/tcpsocket.h \
    app/webserver.h \
    app/sqlserver.h

SOURCES += \
    app/lqsnowflake.cpp \
    app/main.cpp \
    app/tcpserver.cpp \
    app/tcpsocket.cpp \
    app/webserver.cpp \
    app/sqlserver.cpp


