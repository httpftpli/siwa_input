#include "anniuled.h"
#include "main.h"
#include "timer.h"

LED_RUNERR_PRA Led_RUNERR_Pra = {.state = LED_STAT_NOTREGEST};
LED_CANWORK_PRA Led_CANWORK_Pra;


/*uint8_t GetAnniu(void)
{
   if(GPIO_ReadInputDataBit(ANNIU_PORT,ANNIU_PIN))
   {
      return ANNIU_PUTUP;
   }
   else
   {
      return ANNIU_PUTDOWN;
   }
}*/





void LedInit(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    //output

    GPIO_InitStructure.GPIO_Pin = RUN_ERR_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(RUN_ERR_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = CAN_WORK_PIN;
    GPIO_Init(CAN_WORK_PORT, &GPIO_InitStructure);

    Led_RUNERR_Pra.state = LED_STAT_NOTREGEST;    //RUN;
    LedCtr_RUN_ERR(LED_OFF);
    LedCtr_CAN_WORK(LED_OFF);

}
void AnniuInit(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = ANNIU_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(ANNIU_PORT, &GPIO_InitStructure);

}

void LedCtr_RUN_ERR(uint8_t newstate) {
    if (newstate == LED_ON) {
        GPIO_ResetBits(RUN_ERR_PORT, RUN_ERR_PIN);
        Led_RUNERR_Pra.ledstate = LED_ON;
    } else {
        GPIO_SetBits(RUN_ERR_PORT, RUN_ERR_PIN);
        Led_RUNERR_Pra.ledstate = LED_OFF;
    }
}

void LedCtr_CAN_WORK(uint8_t newstate) {
    if (newstate == LED_ON) {
        GPIO_ResetBits(CAN_WORK_PORT, CAN_WORK_PIN);
        Led_CANWORK_Pra.ledstate = LED_ON;
    } else {
        GPIO_SetBits(CAN_WORK_PORT, CAN_WORK_PIN);
        Led_CANWORK_Pra.ledstate = LED_OFF;
    }
}

void ledProcess(void) {
    everydiffdo(unsigned int, __scanled_timetick, timerTick1ms) {
        //run err led process
        switch(Led_RUNERR_Pra.state){
        case LED_STAT_RUN:
            if (Led_RUNERR_Pra.timecount <= 0) {
                if (Led_RUNERR_Pra.ledstate == LED_ON) {
                    LedCtr_RUN_ERR(LED_OFF);
                    Led_RUNERR_Pra.timecount = RUNLED_OFFTIME;
                } else {
                    LedCtr_RUN_ERR(LED_ON);
                    Led_RUNERR_Pra.timecount = RUNLED_ONTIME;
                }
            } else {
                Led_RUNERR_Pra.timecount--;
            }
            break ;
        case LED_STAT_WAITADRR:
            if (Led_RUNERR_Pra.ledstate == LED_OFF) {
                LedCtr_RUN_ERR(LED_ON);
            }
            break ;
        case LED_STAT_NOTREGEST:
            if (Led_RUNERR_Pra.timecount == 0) {
                if (Led_RUNERR_Pra.ledstate == LED_ON) {
                    LedCtr_RUN_ERR(LED_OFF);
                    Led_RUNERR_Pra.timecount = RUNLED_OFFTIME_No_Regedit;
                } else {
                    LedCtr_RUN_ERR(LED_ON);
                    Led_RUNERR_Pra.timecount = RUNLED_ONTIME_No_Regedit;
                }
            } else {
                Led_RUNERR_Pra.timecount--;
            }
            break;
        default:
            break;
        }
        //can bus tx rx led process
        if (Led_CANWORK_Pra.timecount > 0) {
            Led_CANWORK_Pra.timecount--;
        } else {
            LedCtr_CAN_WORK(LED_OFF);
        }
    }
}


void ledCanTxRxON(){
    Led_CANWORK_Pra.timecount = LED_CAN_ONTIME;
    LedCtr_CAN_WORK(LED_ON);
}


void ledRunSetState(uint32 stat){
    Led_RUNERR_Pra.state = stat;
    Led_RUNERR_Pra.timecount = 0;
}
