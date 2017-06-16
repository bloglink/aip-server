#ifndef DEFINE_H
#define DEFINE_H

#define ADDR 6000
#define GUEST_LOGIN             1000 //登录
#define GUEST_LOGIN_ERROR       1001 //登录出错
#define GUEST_PUT_HEAD          1002 //服务器获取文件信息
#define GUEST_GET_HEAD          1003 //客户端获取文件信息
#define GUEST_DISPLAY           1004 //显示信息
#define GUEST_DROPED            1005 //客户端下线
#define ADMIN_LOGIN             1006 //客户端登录
#define ADMIN_LOGIN_ERROR       1007 //登录出错
#define ADMIN_LOGIN_SUCCESS     1008 //登录成功
#define BUILD_TRANSMIT          1102 //建立转发
#define BREAK_TRANSMIT          1103 //取消转发
#define FILE_HEAD               2000 //文件头
#define FILE_DATA               2001 //文件内容
#define FILE_SUCCESS            2002 //文件发送成功
#define FILE_ERROR              2003 //文件内容错误
#define FILE_HEAD_ERROR         2009 //文件头错误
#define SHELL_CMD               2004 //发送系统命令
#define SHELL_DAT               2005 //系统命令返回
#define ONLINE_DEVICES          2006 //在线设备列表
#define SERVER_FILES            2007 //服务器文件列表
#define HEART_BEAT              6000 //心跳

#define WIN_CMD_SWITCH  7000
#define WIN_CMD_SHOW    7001
#define WIN_CMD_RESULT  7002
#define WIN_CMD_JUDGE   7003
#define WIN_CMD_TEMP    7004
#define WIN_WAVE_BYTE   7005
#define WIN_WAVE_TEST   7006
#define WIN_WAVE_ITEM   7007
#define WIN_CMD_DEBUG   7008
#define WIN_CMD_WAVE    7009

#define CAN_DAT_GET 8000
#define CAN_DAT_PUT 8001

#define CTRL_CMD_START 9000
#define CTRL_CMD_STOP  9001

#define  ResCu    0.0039  //  铜材料
#define  ResCu_Al 0.0041  //  铜铝材料
#define  ResAl    0.0043  //  铝材料




#ifdef __arm__
#define GLOBAL_SET "/mnt/nandflash/AIP/Sys.ini"
#else
#define GLOBAL_SET "settings/global.ini"
#endif

#define LOCAL

#define CAN_ID_DLR       0x41         // 电阻板
#define CAN_ID_DLR_WAVE  0x441        // 电阻板反嵌波形
#define CAN_ID_IR        0x61         // 耐压板
#define CAN_ID_IMP       0x81         // 匝间板
#define CAN_ID_IMP_WAVE  0x481        // 匝间板波形
#define CAN_ID_13OUT     0x261        // 输出板13
#define CAN_ID_14OUT     0x281        // 输出板14
#define CAN_ID_15OUT     0x2A1        // 输出板15
#define CAN_ID_16OUT     0x2C1        // 输出板16
#define CAN_ID_17OUT     0x2E1        // 输出板17

#define CAN_CMD_STATE        0x00     //上传状态
#define CAN_CMD_RESULT_ONE   0x01     //上传结果1
#define CAN_CMD_RESULT_TWO   0x02     //上传结果2
#define CAN_CMD_WAVE         0x03     //上传波形
#define CAN_CMD_DEBUG        0x06     //上传调试参数
#define CAN_CMD_VERSION      0x08     //上传版本

#define MSG_VERSION 0x0001

typedef QMap<QString,quint16> TcpMap;

//extern


#endif // DEFINE_H
