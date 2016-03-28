#ifndef MY_DEFINE_H
#define MY_DEFINE_H

#include <QDebug>

#define DB_PATH "/home/link/aip-server/aip.db"

#define W_ROW 50
#define W_COL 6

#define MAX_CLIENT 1000

struct openFileStruct
{
    QString filePath;
    QString fileName;
};

struct clientInfo   //客户端信息
{
    bool isFree;
    bool isInit;
    QString ID;
    QString IP;
    QString NO;
    QString MAC;
    QString PORT;
    QString TIME;
    QString VERSION;
    QString PARAM;
    int heart;
};

const quint8 send_msg           = 0x00;

const quint8 sys_reboot         = 0x01;
const quint8 sys_heart          = 0x02;
const quint8 sys_date           = 0x03;
const quint8 sys_time           = 0x04;
const quint8 sys_ip             = 0x05;
const quint8 sys_num            = 0x06;
const quint8 sys_mac            = 0x07;
const quint8 sys_version        = 0x08;
const quint8 sys_state          = 0x09;
const quint8 sys_config         = 0x10;
const quint8 sys_result         = 0x11;

//const quint8 set_data;
//const quint8 set_time;
//const quint8 set_num;
//const quint8 set_net;
//const quint8 set_config;

const quint8 send_type_msg      = 0x55;
const quint8 send_type_file     = 0x56;
const quint8 send_type_name     = 0x58;
const quint8 send_type_size     = 0x59;
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
