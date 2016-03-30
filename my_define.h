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
    bool isFree;
    bool isInit;
    QString IP;
    QString NO;
    QString MAC;
    QString PORT;
    QString TIME;
    QString VERSION;
    QString PARAM;
};
const quint16 max_client   = 1000;

const quint64 query_ip      = 1000;
const quint64 query_num     = 1001;
const quint64 query_mac     = 1002;
const quint64 query_version = 1003;
const quint64 query_date    = 1004;
const quint64 query_time    = 1005;
const quint64 query_stata   = 1006;
const quint64 query_config  = 1007;

const quint64 reply_ip      = 2000;
const quint64 reply_num     = 2001;
const quint64 reply_mac     = 2002;
const quint64 reply_version = 2003;
const quint64 reply_date    = 2004;
const quint64 reply_time    = 2005;
const quint64 reply_state   = 2006;
const quint64 reply_config  = 2007;

const quint64 send_file     = 4000;
const quint64 send_name     = 4001;
const quint64 send_size     = 4002;
const quint64 send_md5      = 4003;
const quint64 send_data     = 4004;
const quint64 send_time     = 4005;
const quint64 send_state    = 4006;
const quint64 send_config   = 4007;
const quint64 send_list     = 4008;
const quint64 send_heart    = 4009;

const quint8 send_type_msg      = 0x55;
const quint8 send_type_file     = 0x56;
const quint8 send_type_name     = 0x58;
const quint8 send_type_size     = 0x59;
const quint8 send_type_md5      = 0x5a;
const quint8 send_type_file_end = 0x57;

const quint8 send_type_reboot   = 0x00;

const quint8 send_type_ip       = 0x01;
const quint8 send_type_No       = 0x02;
const quint8 send_type_mac      = 0x03;
const quint8 send_type_version  = 0x04;
const quint8 send_type_heart    = 0x05;

const quint8 send_type_test     = 0x10;
const quint8 send_type_state    = 0x11;
const quint8 send_type_config   = 0x12;
const quint8 send_type_network  = 0x13;
const quint8 send_type_result   = 0x14;

const quint8 reply_type_msg      = 0x80;

const quint8 reply_type_reboot   = 0x00 + reply_type_msg;

const quint8 reply_type_ip       = 0x01 + reply_type_msg;
const quint8 reply_type_No       = 0x02 + reply_type_msg;
const quint8 reply_type_mac      = 0x03 + reply_type_msg;
const quint8 reply_type_heart    = 0x05 + reply_type_msg;
const quint8 reply_type_version  = 0x04 + reply_type_msg;

const quint8 reply_type_test     = 0x10 + reply_type_msg;
const quint8 reply_type_state    = 0x11 + reply_type_msg;
const quint8 reply_type_config   = 0x12 + reply_type_msg;
const quint8 reply_type_network  = 0x13 + reply_type_msg;
const quint8 reply_type_result   = 0x14 + reply_type_msg;

const quint8 state_upper   = 0;
const quint8 state_lower   = 1;
const quint8 state_error   = 2;
const quint8 state_test    = 3;
const quint8 state_config  = 4;

#endif // MY_DEFINE_H
