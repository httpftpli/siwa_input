#include "stm32f0xx.h"
#include "timer.h"
#include "type.h"

#define ANNIU_PORT  GPIOC
#define ANNIU_PIN   GPIO_Pin_13

#define RUN_ERR_PORT  GPIOB
#define RUN_ERR_PIN   GPIO_Pin_13

#define CAN_WORK_PORT  GPIOB
#define CAN_WORK_PIN   GPIO_Pin_12

#define LED_ON        1
#define LED_OFF       0

#define LED_STAT_RUN               2
#define LED_STAT_WAITADRR          1
#define LED_STAT_NOTREGEST  	   0

#define RUNLED_ONTIME           300
#define RUNLED_OFFTIME          300

#define RUNLED_ONTIME_No_Regedit	20
#define RUNLED_OFFTIME_No_Regedit	1500

#define LED_CAN_ONTIME              10

#define ANNIU_PUTDOWN           1;
#define ANNIU_PUTUP             0;

typedef struct __led_runerr_pra{
  uint8_t  state;                //0表示正常运行，1表示等待编址，2表示出错
  uint8_t  ledstate;
  int timecount;
}LED_RUNERR_PRA;

typedef struct __led_canwork_pra{
  uint8_t  state;
  uint8_t  ledstate;
  uint16_t timecount;
}LED_CANWORK_PRA;

extern LED_RUNERR_PRA   Led_RUNERR_Pra;
extern LED_CANWORK_PRA  Led_CANWORK_Pra;
extern void LedInit(void);
extern void AnniuInit(void);
extern void LedCtr_RUN_ERR(uint8_t newstate);
extern void LedCtr_CAN_WORK(uint8_t newstate);
extern uint8_t  GetAnniu(void);
extern void ledProcess(void);
extern void ledCanTxRxON();
extern void ledRunSetState(uint32 stat);



