//#include "step.h"
#include "can.h"
#include "main.h"
#include "io_input.h"

#include "cancmd.h"
#include "anniuled.h"
#include "timer.h"
#include "algorithm.h"
#include "can_dmp.h"
#include "can_wp.h"






uint8_t CanCmd_setAlarm(CAN_WP *wp) {
    unsigned cmdnum = wp->dlc;
    if (cmdnum % 4 != 0) {
        return 0;
    }
    cmdnum = cmdnum / 4;
    for (int i = 0; i < cmdnum; i++) {
        CAN_CMD_SET_ALARM *cmd = (CAN_CMD_SET_ALARM *)wp->data + i;
        unsigned int bit = cmd->bit;
        if (bit >= Max_Input_Single) {
            break;
        }
        if (cmd->en == 0) {
            io_scan[bit].en = 0;
            io_scan[bit].stat = IO_SCAN_STAT_NO;
            io_scan[i].debouceCnt = 0;
        } else {
            if (cmd->time != 0xffff) {
                io_scan[bit].en = cmd->en ? IO_SCAN_STAT_EN_ALRAM : 0;
                io_scan[bit].val = !!cmd->val;
                io_scan[bit].begintime = getTimerTick() + cmd->time / 2;
                io_scan[bit].timelast = cmd->time / 2;
                io_scan[bit].stat = IO_SCAN_STAT_NO;
                io_scan[i].debouceCnt = 0;
            } else {
                io_scan[bit].en = cmd->en ? IO_SCAN_STAT_EN_ALRAM : 0;
                io_scan[bit].val = !!cmd->val;
                //io_scan[bit].begintime =  getTimerTick() + cmd->time/2;
                io_scan[bit].timelast = cmd->time;
                io_scan[bit].stat = IO_SCAN_STAT_OK;
                //io_scan[i].debouceCnt = 0;
            }
        }
    }
    return 0;
}



void CANCMD_test_time(CAN_WP *wp) {
    unsigned cmdnum = wp->dlc;
    if (cmdnum % sizeof(CAN_CMD_TEST_TIME) != 0) {
        return;
    }
    cmdnum = cmdnum / sizeof(CAN_CMD_TEST_TIME);
    for (int i = 0; i < cmdnum; i++) {
        CAN_CMD_TEST_TIME *cmd = (CAN_CMD_TEST_TIME *)(wp->data) + i;
        unsigned int bit = cmd->bit;
        if (bit > Max_Input_Single) {
            break;
        }
        if (io_scan[bit].en == 0) {
            io_scan[bit].en = IO_SCAN_STAT_EN_TEST;
            io_scan[bit].val = !!cmd->val;
            io_scan[bit].timelast = cmd->timeout;
            io_scan[bit].stat = IO_SCAN_STAT_NO;
            io_scan[bit].debouceCnt = 0;
            io_scan[bit].begintime = getTimerTick();
        }
    }
    return;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
uint8_t CANCMD_readvalue(CAN_WP *wp) {

    unsigned int io = io_bitmap;

    DEFINE_CAN_WP_FRAME(twp);
    //twp.srcid = DeviceCanAddr;
    twp.funcode = CANCMD_READ_VALUE;
    twp.desid = wp->srcid;
    twp.dlc = 4;
    *(unsigned int *)&twp.data = io;
    if (wpSend(&twp)) {
        return 1;
    }
    return 0;
}


RINGBUF canCmdRcvBuf;

void CANCMD_buf_init() {
    static CanRxMsg __msg[15];
    ringBufInit(&canCmdRcvBuf, __msg, sizeof(CanRxMsg), lenthof (__msg));
}

void CANCMD_set_alarm_enable(CAN_WP *wp) {
    if (wp->dlc != 4) {
        return;
    }
    io_alarm_enable_bitmap = *(unsigned int *)&wp->data & INPUT_MASK;

    DEFINE_CAN_WP_FRAME(twp);
    //twp.srcid = DeviceCanAddr;
    twp.funcode = CANCMD_SET_ALARM_ENABLE;
    twp.desid = wp->srcid;
    twp.dlc = 4;
    *(unsigned int *)&twp.data = io_alarm_enable_bitmap;
    wpSend(&twp);
}



void doCmdWp(CAN_WP *wp) {
    if (CAN_WP_GET_ID(wp->desid)==0) {
        return;
    }
    if (DeviceCanAddr==0) {
        return;
    }
    switch (wp->funcode) {
    case CANCMD_SETALARM:
        CanCmd_setAlarm(wp);
        break;
    case CANCMD_TEST_TIME:
        CANCMD_test_time(wp);
        break;
    case CANCMD_READ_VALUE:
        CANCMD_readvalue(wp);
        break;
    case CANCMD_SET_ALARM_ENABLE:
        CANCMD_set_alarm_enable(wp);
        break;
    default:
        break;
    }
}


void ScanCmd(void) {
    CanRxMsg msg;
    if (ringBufPop(&canCmdRcvBuf, &msg)) {
        if (IS_CANMSG_WPCMD(&msg)) {
            CAN_WP wp;
            CANRxMSG_TO_CANWP(&wp, &msg);
            doCmdWp(&wp);
        } else if(IS_CANMSG_DMPCMD_RCV(&msg)){
            CAN_DMP dmp;
            CANRxMSG_TO_CANDMP(&dmp,&msg);
            doCmdDmp(&dmp);
        }
    }
}


