#ifndef __CAN__DMP__H
#define __CAN__DMP__H

#include "string.h"


#define CAN_WP_DEV_TYPE_MAIN    0x01UL
#define CAN_WP_DEV_TYPE_MOTOR   0x02UL
#define CAN_WP_DEV_TYPE_INPUT   0x03UL
#define CAN_WP_DEV_TYPE_QIFA    0x04UL
#define CAN_WP_DEV_TYPE_SIFU    0x05UL


/**************config by user */
#define DMP_DEV_TYPE   CAN_WP_DEV_TYPE_INPUT
/***************end************/

#define CAN_DMP_BROADCAST_ID     0X7FFFFFF

#define CANCMD_READSOFTVER       		0x01
#define CANCMD_CHECKID				    0X02
#define CANCMD_SETADRR       			0XFD
#define CANCMD_GOTOAPP       			0xFC
#define CANCMD_GOTOIAP      	 		0xFB
#define CANCMD_WAITSETADD			    0XFA

typedef struct {
    unsigned int uid:27;
    unsigned int dir:1;
    unsigned int flag:1;
    unsigned int candir:1;
    unsigned int xtd:1;
    unsigned int dlc;
    unsigned char data[8];
}CAN_DMP;


extern uint32_t Flash_Device_ID_STM ;
static inline void __candmpinit(CAN_DMP *frame){
    frame->uid = Flash_Device_ID_STM;
}
#define DEFINE_CAN_DMP_FRAME(frame) CAN_DMP frame =  \
              {.flag = 0,.candir = 0,.xtd=1,};\
              __candmpinit(&frame)




static inline void CANRxMSG_TO_CANDMP(CAN_DMP *cmd,CanRxMsg *msg){
    cmd->uid = msg->ExtId & 0x7ffffff;
    cmd->flag = 0;
    cmd->dir = 0;
    cmd->xtd = 1;
    cmd->dlc = msg->DLC;
    memcpy(cmd->data,msg->Data,lenthof(msg->Data));
}



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
extern void CanCmd_GoToIap(CAN_DMP *dmp);
extern uint8_t CanCmd_SetAddr(CAN_DMP *dmp);
extern void CanCmd_APP_Return_SetAdrr(void);
extern void CanCmd_WaitSetAdd(CAN_DMP *dmp);
extern void CanCmd_Check_Device(CAN_DMP *dmp);
extern void CanCmd_ReadSoftVer(CAN_DMP *dmp);
extern void doCmdDmp(CAN_DMP *dmp);
extern uint8_t dmpSend(CAN_DMP *dmp);

#endif