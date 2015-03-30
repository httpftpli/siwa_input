/**
  ******************************************************************************
  * @file    Project/STM32F0xx_StdPeriph_Templates/main.h
  * @author  MCD Application Team
  * @version V1.3.1
  * @date    17-January-2014
  * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"


#define   T1msCNT  1000;
#define   T10msCNT  10000;


#define STM32F042C6
#if  !defined (STM32F042C4)
#define   APP_ADDREND             ((uint32_t)0x08008000)       //STM32F042C6      APPÙÀåa÷œÏ„»A≥u
#else
#define   APP_ADDREND             ((uint32_t)0x08004000)       //STM32F042C4      APPÙÀåa÷œÏ„»A≥u
#endif



#define   IAP_ADDR                ((uint32_t)0x08000000)       
#define   APP_ADDR                ((uint32_t)0x08002400)      
//#define   APP_ADDREND             ((uint32_t)0x08008000)      
#define   FLASH_PAGE_SIZE         ((uint32_t)0x00000400)
#define   IAPLICATION_ADDRESS     ((uint32_t)0x08000000)
#define   APPLICATION_ADDRESS     ((uint32_t)APP_ADDR)     
#define   PROENDMARK              ((uint32_t)(APP_ADDREND-4))     

#define   POWERUP_APPTOIAP  0x55aa5aa5
#define   POWERUP_IAPTOAPP  0xaa55a55a




//extern const uint8_t Ver[36];

extern uint32_t Flash_Device_ID_STM ;					//STM32 Œ®“ªID

void IWDG_Config(void);


#define everydiffdo(TYPE,MARK,val)\
            static TYPE __old ## MARK = 0;\
            for (TYPE MARK = val; MARK!=__old ## MARK;\
              __old ## MARK=MARK)



#define lenthof(array) (sizeof((array))/sizeof((array[0])))


#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
