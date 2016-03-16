#ifndef MY_DEFINE_H
#define MY_DEFINE_H

#include <QDebug>

#define KEYBOARD_X 660
#define KEYBOARD_Y 240
#define ICON 64
#define DB_PATH "/mnt/nfs/aip.db"

#define W_MAIN  0x00
#define W_HOME  0x00
#define W_SYST  0x01
#define W_MODL  0x02
#define W_DATA  0x03
#define W_TEST  0x04
#define W_CONF  0x05
#define W_COLR  0x06
#define W_HELP  0x07
#define W_HIDE  0x08

#define W_INIT  0x09
#define W_EXIT  0x10
#define W_ERROR 0x11

#define W_ALL   0x00
#define W_RES   0x01
#define W_OPP   0x02
#define W_INS   0x03
#define W_ACV   0x04
#define W_DCV   0x05
#define W_ITT   0x06

#define W_SYST_EXIT   0x00
#define W_SYST_SAVE   0x01
#define W_SYST_DATE   0x02
#define W_SYST_MAIN   0x03
#define W_SYST_FIRM   0x04
#define W_SYST_BUTTON 3

#define W_MODL_EXIT   0x00
#define W_MODL_IMPORT 0x01
#define W_MODL_EXPORT 0x02
#define W_MODL_ADD    0x03
#define W_MODL_DEL    0x04
#define W_MODL_BUTTON 5

#define W_DATA_EXIT   0x00
#define W_DATA_EXPORT 0x03
#define W_DATA_CLEAR  0x04
#define W_DATA_BUTTON 3

#define W_TEST_EXIT   0x00
#define W_TEST_CONF   0x03
#define W_TEST_START   0x04
#define W_TEST_BUTTON 2

#define W_CONF_EXIT 0x00
#define W_CONF_SAVE 0x01
#define W_CONF_EASY 0x02
#define W_CONF_CULL 0x03
#define W_CONF_ADD  0x04
#define W_CONF_DEL  0x05
#define W_CONF_ERR  0x06
#define W_CONF_RESULT 0x07
#define COLOR_01    0x81
#define COLOR_02    0x82
#define COLOR_03    0x83
#define COLOR_04    0x84
#define COLOR_05    0x85
#define COLOR_06    0x86
#define COLOR_07    0x87
#define COLOR_08    0x88
#define COLOR_09    0x89
#define COLOR_10    0x8A
#define COLOR_11    0x8B
#define COLOR_12    0x8C
#define W_CONF_MODL 0x90
#define W_CONF_CHECK 0x10
#define W_CONF_ALL  0x11

#define W_HELP_EXIT 0x00

#define TEST_FREE	0x00
#define RES_START   0x01
#define RES_WAIT    0x02
#define RES_END     0x03

#define CONF paramList[0]
#define RES  paramList[1]
#define OPP  paramList[2]
#define INS  paramList[3]
#define ACV  paramList[4]
#define DCV  paramList[5]
#define ITT  paramList[6]
#define OTHER 20
#define CONF_OTHER 20
#define NUM_PROJ 30

#define COLOR_NUM 12
#define COLOR_COL 3
#define COLOR_SPC 1


#define MODL_ROW 5
#define MODL_SPC 1

#define FILE_COL 3

#define RES_NUM 20
#define RES_COL 5
#define RES_ROW 8
#define RES_OTHER 5

#define OPP_NUM 820
#define OPP_ROW 8
#define OPP_COL 3
#define OPP_OTHER 0
#define OPP_WAVE 4
#define WAVE_WIDTH 200
#define WAVE_HEIGHT 64

#define INS_NUM 20
#define INS_OTHER 4

#define ACV_NUM 20
#define ACV_OTHER 6

#define DCV_NUM 20
#define DCV_OTHER 5

#define ITT_NUM 420
#define ITT_ROW 8
#define ITT_COL 8
#define ITT_OTHER 0
#define ITT_WAVE 10

#define NUM_RES 20
#define NUM_OPP 820
#define NUM_INS 20
#define NUM_ACV 20
#define NUM_DCV 20
#define NUM_ITT 20
#define NUM_WAVE 800

#define CAN_BUS_FREE    0x00
#define CAN_BUS_BUZY    0x01

#define CAN_RES_DATA    0x10
#define CAN_RES_SEND    0x11
#define CAN_RES_STATE   0x13

#define CAN_OPP_WAVE    0x20
#define CAN_OPP_EXIT    0x21
#define CAN_OPP_DATA    0x22

#define CAN_ITT_DATA    0x60
#define CAN_ITT_WAVE    0x61
#define CAN_ITT_EXIT    0x62

#endif // MY_DEFINE_H
