/**
  ******************************************************************************
  * @file    Project/STM32F0xx_StdPeriph_Templates/main.c
  * @author  MCD Application Team
  * @version V1.3.1
  * @date    17-January-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "type.h"
#include "main.h"
#include "io_input.h"
//#include "step.h"
#include "can.h"
#include "cancmd.h"
#include "anniuled.h"
#include "flash id.h"
#include "timer.h"

const uint32_t ProEndMark @(PROENDMARK) = 0x5faff55f;
__IO uint32_t Powerup @(0x200017f0);

uint32_t Flash_Device_ID_STM;                  //STM32 唯一ID
#if(STM32F0x_FLASHID_READ)
//uint32_t FLASHID_X_Y_coordinates;		//UID[ 31: 0 ]  X and Y coordinates on the wafer expressed in BCD format
//uint32_t FLASHID_X_coordinates;
//uint32_t FLASHID_Y_coordinates;
//uint8_t WAF_NUM;						//UID[39:32]: WAF_NUM[7:0]	Wafer number (8-bit unsigned number)
//uint32_t LOT_NUM_l;					//UID[63:40]: LOT_NUM[23:0]	Lot number (ASCII encoded)
//uint32_t LOT_NUM_h;					//UID[95:64]: LOT_NUM[55:24]	Lot number (ASCII encoded)
//long long LOT_NUM;
//uint32_t buf_change[8];
#endif


uint32_t send_end_status = 0;      //发送最后一次状态
uint32_t send_end_count_delay = 0;
int main(void) {
    memcpy((void *)0x20000000,(void *)(APPLICATION_ADDRESS),48*4);//copy vectortable
    /* Enable the SYSCFG peripheral clock*/
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    /* Remap SRAM at 0x00000000 */
    SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
    Flash_Device_ID_STM = Get_Flash_ID();

    //系统外部时钟8M,PLL6倍，48M
    SystemInit();
    timerTick1msInit();
    LedInit();
    AnniuInit();
    Io_input_Init();
    ledRunSetState(LED_STAT_NOTREGEST);
    CANCMD_buf_init();
    Canini();
    io_scan_init();

    while (1) {
       ScanCmd();
       ioAlarmProcess();
       ioTesttimeProcess();
       ioScan();
       ledProcess();
    }
}




#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line) {
    /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1) {
    }
}
#endif

/**
  * @}
  */
//////////////////////////////////
/*void Timer14ini(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);


    TIM_TimeBaseStructure.TIM_Period = T1msCNT;
    //TIM_TimeBaseStructure.TIM_Period = T10msCNT;
    TIM_TimeBaseStructure.TIM_Prescaler = 47; //48MHz / (47+1) = 1MHz about 1us
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM14, TIM_EventSource_Update, ENABLE);
    TIM_Cmd(TIM14, ENABLE);
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}*/

void IWDG_Config(void) {
    /* IWDG timeout equal to 250 ms (the timeout may varies due to LSI frequency
    dispersion) */
    /* Enable write access to IWDG_PR and IWDG_RLR registers */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    /* IWDG counter clock: LSI/32 */
    IWDG_SetPrescaler(IWDG_Prescaler_32);

    /* Set counter reload value to obtain 250ms IWDG TimeOut.
    Counter Reload Value = 250ms/IWDG counter clock period
    = 250ms / (LSI/32)
    = 0.25s / (LsiFreq/32)
    = LsiFreq/(32 * 4)
    = LsiFreq/128
    LsiFreq=40k;
    */
    IWDG_SetReload(312);

    /* Reload IWDG counter */
    IWDG_ReloadCounter();

    /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
    IWDG_Enable();
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
