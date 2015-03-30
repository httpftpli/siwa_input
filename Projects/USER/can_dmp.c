#include "can.h"
#include "main.h"
#include "io_input.h"
#include "can_wp.h"

#include "cancmd.h"
#include "anniuled.h"
#include "timer.h"
#include "algorithm.h"
#include "can_dmp.h"




typedef void (*pFunction)(void);
pFunction Jump_To_Application;

uint8_t DeviceCanAddr = 0;

const uint8_t Ver[] = { "SoftVer-A.SINA.PB-A.01.00.00&"__DATE__ };

#define BOOT_FLAG_APPTOIAP (*(unsigned int *)(0xff))

void CanCmd_GoToIap(CAN_DMP *dmp) {
    if (dmp->dlc!=1) {
        return;
    }
    DEFINE_CAN_DMP_FRAME(dmpt);
    dmpt.dlc = 1;
    dmpt.data[0] = CANCMD_GOTOIAP;
    dmpSend(&dmpt);
    __disable_irq();
    BOOT_FLAG_APPTOIAP = POWERUP_APPTOIAP;
    Jump_To_Application = (pFunction)(IAPLICATION_ADDRESS + 4);
    __set_MSP(*(__IO uint32_t *)IAPLICATION_ADDRESS);
    Jump_To_Application();
}


#define Ver_No_Send  8 //  "SoftVer-" should not send

void CanCmd_ReadSoftVer(CAN_DMP *dmp) {
    uint16_t i;
    uint8_t j;
    uint8_t length;
    uint8_t num;
    DEFINE_CAN_DMP_FRAME(dmpt);
    dmpt.data[0] = CANCMD_READSOFTVER;

    length = sizeof Ver;
    length = length + 1 - Ver_No_Send + 1;      //实际要发送的数据长度  每包实际数据为6个
    num = DIVUP(length, 6);
    for (j = 0; j < num; j++) {
        dmpt.data[1] = ((num & 0xf) << 4) + j;
        if (j == (num - 1)) {
            //最高一包数据
            dmpt.dlc = (length % 6) + 2;
            dmpt.data[dmpt.dlc - 1] = DeviceCanAddr;
            for (i = 2; i < (dmpt.dlc - 1); i++) {
                dmpt.data[i] = Ver[8 + j * 6 + i - 2];
            }
        } else {
            dmpt.dlc = 8;
            for (i = 2; i < 8; i++) {
                dmpt.data[i] = Ver[8 + (i - 2) + j * 6];
            }
        }
        dmpSend(&dmpt);
    }
}


#define CAN_FILTER_IDX_DATA    0X04
#define CAN_MASK_IDX_DATA      0X06

uint8_t CanCmd_SetAddr(CAN_DMP *dmp) {
    if ((dmp->uid != Flash_Device_ID_STM) || (dmp->dlc!=3)) {
        return 0;
    }

    uint8_t addr;
    addr = dmp->data[2] << 8 | dmp->data[1];
    if ((addr == 0) || (addr == CAN_WP_ID_ALL_BROADCAST) ||
        (CAN_WP_GET_TYPE(addr) != CAN_WP_DEV_TYPE_INPUT)) {
        return 0;
    }

    DeviceCanAddr = addr;
    unsigned int mask = (0x3ff<<18 | 1<<28)<<3 | CAN_MASK_IDX_DATA;
    unsigned int filter = (CAN_WP_ID(CAN_WP_DEV_TYPE_INPUT, addr) << 18 | 1 << 28) << 3
            | CAN_FILTER_IDX_DATA;
    canAddfilter(filter,mask,4);
    ledRunSetState(LED_STAT_RUN);

    DEFINE_CAN_DMP_FRAME(dmpt);
    dmpt.dlc = 3;
    dmpt.data[0] = CANCMD_SETADRR;
    dmpt.data[1] = (uint8)DeviceCanAddr;
    dmpt.data[2] = (uint8)(DeviceCanAddr >> 8);
    dmpSend(&dmpt);
    return 1;
}


void CanCmd_WaitSetAdd(CAN_DMP *dmp) {
    if ((dmp->uid != Flash_Device_ID_STM) || dmp->dlc!=2) {
        return;
    }
    if (dmp->data[1] == 1) {
        ledRunSetState(LED_STAT_WAITADRR);            //让LED灯进入等待配对模式 LED闪加快
    } else {
        if ((DeviceCanAddr != 0) && (DeviceCanAddr != 0xff)) {
            ledRunSetState(LED_STAT_RUN);
        }else{
            ledRunSetState(LED_STAT_NOTREGEST);
        }
    }
    DEFINE_CAN_DMP_FRAME(dmpt);
    dmpt.dlc = 1;
    dmpt.data[0] = CANCMD_WAITSETADD;
    dmpSend(&dmpt);
}




void CanCmd_Check_Device(CAN_DMP *dmp) {
    DEFINE_CAN_DMP_FRAME(dmpt);
    dmpt.dlc = 2;
    dmpt.data[0] = CANCMD_CHECKID;
    dmpt.data[1] = (uint8)DeviceCanAddr;
    dmpt.data[2] = (uint8)(DeviceCanAddr >> 8);
    dmpSend(&dmpt);
}


void doCmdDmp(CAN_DMP *dmp) {
    switch (dmp->data[0]) {
    case CANCMD_READSOFTVER:
        CanCmd_ReadSoftVer(dmp);
        break;
    case CANCMD_CHECKID:
        CanCmd_Check_Device(dmp);
        break;
    case CANCMD_WAITSETADD:
        CanCmd_WaitSetAdd(dmp);
        break;
    case CANCMD_SETADRR:
        CanCmd_SetAddr(dmp);
        break;
    case CANCMD_GOTOIAP:
        CanCmd_GoToIap(dmp);
        break;
    default:
        break;
    }
}


uint8_t dmpSend(CAN_DMP *dmp){
    CanTxMsg msg;
    msg.ExtId = dmp->uid | 1<<27 | 0<<28;
    msg.RTR = CAN_RTR_DATA;
    msg.IDE = CAN_ID_EXT;
    msg.DLC = dmp->dlc;
    memcpy(msg.Data,dmp->data,lenthof(msg.Data));
    return CanSend(&msg);
}




