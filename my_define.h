#ifndef MY_DEFINE_H
#define MY_DEFINE_H

#include <QDebug>

#define DB_PATH "/home/link/aip-server-build/aip.db"

#define W_ROW 50
#define W_COL 6

struct openFileStruct
{
    QString filePath;
    QString fileName;
};

struct clientInfo   //客户端信息
{
    QString Hardware;
    QString Address;
    QString Version;
    QString Number;
    QString Param;
    QString Time;
    bool isFree;
    bool isInit;
};
const quint16 max_client    = 1000;
const quint64 serverIndex   = max_client;

const quint64 query_num     = 1001;
const quint64 query_mac     = 1002;
const quint64 query_version = 1003;
const quint64 query_date    = 1004;
const quint64 query_time    = 1005;
const quint64 query_stata   = 1006;
const quint64 query_config  = 1007;

const quint64 send_num      = 2001;
const quint64 send_mac      = 2002;
const quint64 send_version  = 2003;
const quint64 send_date     = 2004;
const quint64 send_time     = 2005;
const quint64 send_state    = 2006;
const quint64 send_config   = 2007;
const quint64 send_file     = 4000;
const quint64 send_name     = 4001;
const quint64 send_size     = 4002;
const quint64 send_md5      = 4003;
const quint64 send_list     = 4008;
const quint64 send_heart    = 4009;
const quint64 send_test     = 4010;
const quint64 send_error    = 4011;

const quint8 state_upper   = 0;
const quint8 state_lower   = 1;
const quint8 state_error   = 2;
const quint8 state_test    = 3;
const quint8 state_config  = 4;

#endif // MY_DEFINE_H
