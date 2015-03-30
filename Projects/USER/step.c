#include "step.h"  
#include "can.h"  

const GPIOGROUP  MiduMotoRef[MiDuMotoNum]={
 	{Moto1_REF_PORT,Moto1_REF_PIN},
	{Moto2_REF_PORT,Moto2_REF_PIN},
	{Moto3_REF_PORT,Moto3_REF_PIN},
        {Moto4_REF_PORT,Moto4_REF_PIN}
};

const GPIOGROUP  MiduMotoFree[MiDuMotoNum]={
 	{Moto1_FREE_PORT,Moto1_FREE_PIN},
	{Moto2_FREE_PORT,Moto2_FREE_PIN},
	{Moto3_FREE_PORT,Moto3_FREE_PIN},
        {Moto4_FREE_PORT,Moto4_FREE_PIN}
};

const GPIOGROUP  MiduMotoStep[MiDuMotoNum]={
 	{Moto1_STEP_PORT,Moto1_STEP_PIN},
	{Moto2_STEP_PORT,Moto2_STEP_PIN},
	{Moto3_STEP_PORT,Moto3_STEP_PIN},
        {Moto4_STEP_PORT,Moto4_STEP_PIN}
};
const GPIOGROUP  MiduMotoDir[MiDuMotoNum]={
 	{Moto1_DIR_PORT,Moto1_DIR_PIN},
	{Moto2_DIR_PORT,Moto2_DIR_PIN},
	{Moto3_DIR_PORT,Moto3_DIR_PIN},
        {Moto4_DIR_PORT,Moto4_DIR_PIN},
};
const GPIOGROUP  MiduMotoZero[MiDuMotoNum]={
 	{Moto1_ZERO_PORT,Moto1_ZERO_PIN},
	{Moto2_ZERO_PORT,Moto2_ZERO_PIN},
	{Moto3_ZERO_PORT,Moto3_ZERO_PIN},
        {Moto4_ZERO_PORT,Moto4_ZERO_PIN}
};




MIDUSTEPCTRPRA   MiduMotoCtrPra[MiDuMotoNum];


void ResetDumu(uint8_t moto,uint8_t cmd)
{
           uint8_t j;
           ////////////////////////////////////////////
            switch(moto)
             {
	      case MIDUMOTO1:{ TIM_ITConfig(TIM1, TIM_IT_CC1 , DISABLE);break;}
	      case MIDUMOTO2:{ TIM_ITConfig(TIM1, TIM_IT_CC2 , DISABLE);break;}
	      case MIDUMOTO3:{ TIM_ITConfig(TIM1, TIM_IT_CC3 , DISABLE);break;}
	      case MIDUMOTO4:{ TIM_ITConfig(TIM1, TIM_IT_CC4 , DISABLE);break;}
	      default:break;
	      }
           //////////////////////////////////////////////
            
           MiduMotoCtrPra[moto].MotoRunDir=OUT_DIR_ZHENG;
	   MiduMotoDirCtr(moto,OUT_DIR_ZHENG);
	   MiduMotoRefCtr(moto,ENABLE);
           if(cmd==0x82)
           {
           MiduMotoFreeCtr(moto,DISABLE);
	   MiduMotoCtrPra[moto].MotoRunState=MOTOSTOPHALF;
           }
           else
           {
               MiduMotoFreeCtr(moto,ENABLE);
	       MiduMotoCtrPra[moto].MotoRunState=MOTOSTOPFERR;
           }
           
	   MiduMotoCtrPra[moto].MotoZeroState[2]=GetMiduMotoZero(moto);
	   MiduMotoCtrPra[moto].MotoZeroState[1]=GetMiduMotoZero(moto);
	   MiduMotoCtrPra[moto].MotoZeroState[0]=GetMiduMotoZero(moto);
	   MiduMotoCtrPra[moto].MotoDumuDangqian=0;
	   MiduMotoCtrPra[moto].MotoDumuPuls=0;
	   MiduMotoCtrPra[moto].MotoPwmPinState=0;
           GetMotoDumuStopData(moto,0xff);          //忽略0位 运行。
	   MiduMotoCtrPra[moto].MotoRunCmpValue=GetMotoCmpValue(MIDUMOTOHZMIN);
	   MiduMotoCtrPra[moto].MotoRunHzDangqian=MIDUMOTOHZMIN;
           MiduMotoCtrPra[moto].MotoRunHzMax=MIDUMOTOHZMIN;
           MiduMotoCtrPra[moto].MotoRunHzMin=MIDUMOTOHZMIN;
	   GetMotoAddTab(moto,MIDUMOTOHZMIN);
           
           
           /////////////////////////////////
           CanCmdLoopBuf.DumoMotoCmdbuf[moto].CanCmdDataPoint=0;
           CanCmdLoopBuf.DumoMotoCmdbuf[moto].CanCmdDoPoint=0;
           CanCmdLoopBuf.DoDumoMotoCmdbuf[moto].busy=0;
           CanCmdLoopBuf.DoDumoMotoCmdbuf[moto].DoCmdTimeCount=0;
           CanCmdLoopBuf.DoDumoMotoCmdbuf[moto].DoCmdTimeout=0;
           CanCmdLoopBuf.DoDumoMotoCmdbuf[moto].CmdCode=0;
           CanCmdLoopBuf.DoDumoMotoCmdbuf[moto].CmdLen=0;
           CanCmdLoopBuf.DoDumoMotoCmdbuf[moto].CmdDataBuf[0]=0;
           CanCmdLoopBuf.DoDumoMotoCmdbuf[moto].CmdDataBuf[1]=0;
           CanCmdLoopBuf.DoDumoMotoCmdbuf[moto].CmdDataBuf[2]=0;
           CanCmdLoopBuf.DoDumoMotoCmdbuf[moto].CmdDataBuf[3]=0;
           CanCmdLoopBuf.DoDumoMotoCmdbuf[moto].CmdDataBuf[4]=0;
           CanCmdLoopBuf.DoDumoMotoCmdbuf[moto].CmdDataBuf[5]=0;
           CanCmdLoopBuf.DoDumoMotoCmdbuf[moto].CmdDataBuf[6]=0;
           CanCmdLoopBuf.DoDumoMotoCmdbuf[moto].CmdDataBuf[7]=0;
           for(j=0;j<CANCMDLOOPBUFMAX;j++)
           {
              
              
              CanCmdLoopBuf.DumoMotoCmdbuf[moto].CanCmdBuf[j].CmdCode=0;
              CanCmdLoopBuf.DumoMotoCmdbuf[moto].CanCmdBuf[j].CmdLen=0;
              CanCmdLoopBuf.DumoMotoCmdbuf[moto].CanCmdBuf[j].CmdDataBuf[0]=0;
              CanCmdLoopBuf.DumoMotoCmdbuf[moto].CanCmdBuf[j].CmdDataBuf[1]=0;
              CanCmdLoopBuf.DumoMotoCmdbuf[moto].CanCmdBuf[j].CmdDataBuf[2]=0;
              CanCmdLoopBuf.DumoMotoCmdbuf[moto].CanCmdBuf[j].CmdDataBuf[3]=0;
              CanCmdLoopBuf.DumoMotoCmdbuf[moto].CanCmdBuf[j].CmdDataBuf[4]=0;
              CanCmdLoopBuf.DumoMotoCmdbuf[moto].CanCmdBuf[j].CmdDataBuf[5]=0;
              CanCmdLoopBuf.DumoMotoCmdbuf[moto].CanCmdBuf[j].CmdDataBuf[6]=0;
              CanCmdLoopBuf.DumoMotoCmdbuf[moto].CanCmdBuf[j].CmdDataBuf[7]=0;
              
           }
           
}


void MotoDumuRunScan(uint8_t Moto)
{
   uint32_t  capture;
   if(MiduMotoCtrPra[Moto].MotoRunState==MOTOWORKING)
   	{
   	   if(MiduMotoCtrPra[Moto].MotoDumuPuls>0)
   	   	{
   	     if(MiduMotoCtrPra[Moto].MotoDumuPuls%2==0)
   	   	   	{
   	   	   	    MiduMotoCtrPra[Moto].MotoZeroState[2]=MiduMotoCtrPra[Moto].MotoZeroState[1];
                            MiduMotoCtrPra[Moto].MotoZeroState[1]=MiduMotoCtrPra[Moto].MotoZeroState[0];
			    MiduMotoCtrPra[Moto].MotoZeroState[0]=GetMiduMotoZero(Moto);
				
   	   	   	    if((MiduMotoCtrPra[Moto].MotoZeroState[2]==MiduMotoCtrPra[Moto].MotoStopData[2])
				 &&(MiduMotoCtrPra[Moto].MotoZeroState[1]==MiduMotoCtrPra[Moto].MotoStopData[1])
				 &&(MiduMotoCtrPra[Moto].MotoZeroState[0]==MiduMotoCtrPra[Moto].MotoStopData[0]))
   	   	   	    	{
   	   	   	    	  //满足零位条件，//结束进入半流 并关闭中断，清PLUS，坐标
   	   	   	    	  MiduMotoCtrPra[Moto].MotoDumuPuls=0;
					  MiduMotoCtrPra[Moto].MotoDumuDangqian=0;
                                           MiduMotoCtrPra[Moto].MotoRunHzDangqian= MiduMotoCtrPra[Moto].MotoRunHzMin;
					  MiduMotoCtrPra[Moto].MotoRunState=MOTOSTOPHALF;
					  MiduMotoRefCtr(Moto,ENABLE);
					  switch(Moto)
					  	{
					  	  case MIDUMOTO1:{ TIM_ITConfig(TIM1, TIM_IT_CC1 , DISABLE);break;}
						  case MIDUMOTO2:{ TIM_ITConfig(TIM1, TIM_IT_CC2 , DISABLE);break;}
						  case MIDUMOTO3:{ TIM_ITConfig(TIM1, TIM_IT_CC3 , DISABLE);break;}
						  case MIDUMOTO4:{ TIM_ITConfig(TIM1, TIM_IT_CC4 , DISABLE);break;}
						  default:break;
					  	}
					  
   	   	   	    	}
				else
					{
					  //正常运行
					  MiduMotoStepCtr(Moto,OUT_UP );
		                          MiduMotoCtrPra[Moto].MotoPwmPinState=1;
					  MiduMotoCtrPra[Moto].MotoDumuPuls--;
					  if(MiduMotoCtrPra[Moto].MotoRunDir==OUT_DIR_FAN)
					  	{
					  	   MiduMotoCtrPra[Moto].MotoDumuDangqian--;
					  	}
					  else
					  	{ 
					  	  MiduMotoCtrPra[Moto].MotoDumuDangqian++;
					  	}
					  
					  /////////////频率处理,加减速处理
                      
					  GetMotoAddValue(Moto);
					  MiduMotoCtrPra[Moto].MotoRunCmpValue=GetMotoCmpValue(MiduMotoCtrPra[Moto].MotoRunHzDangqian);
					  
                       switch(Moto)
                       	{
                       	 case MIDUMOTO1:{capture = TIM_GetCapture1(TIM1)+MiduMotoCtrPra[Moto].MotoRunCmpValue;TIM_SetCompare1(TIM1, capture  );break;}
			 case MIDUMOTO2:{capture = TIM_GetCapture2(TIM1)+MiduMotoCtrPra[Moto].MotoRunCmpValue;TIM_SetCompare2(TIM1, capture  );break;}
			 case MIDUMOTO3:{capture = TIM_GetCapture3(TIM1)+MiduMotoCtrPra[Moto].MotoRunCmpValue;TIM_SetCompare3(TIM1, capture  );break;}
			 case MIDUMOTO4:{capture = TIM_GetCapture4(TIM1)+MiduMotoCtrPra[Moto].MotoRunCmpValue;TIM_SetCompare4(TIM1, capture  );break;}
			 default:break;
                       	}
					  
					  
					}
   	   	   	}
		   else
		   	{
		   	    MiduMotoStepCtr(Moto,OUT_DOWN);
		            MiduMotoCtrPra[Moto].MotoPwmPinState=0;
			   
   			     switch(Moto)
                       	{
                       	 case MIDUMOTO1:{capture = TIM_GetCapture1(TIM1)+MiduMotoCtrPra[Moto].MotoRunCmpValue;TIM_SetCompare1(TIM1, capture  );break;}
			 case MIDUMOTO2:{capture = TIM_GetCapture2(TIM1)+MiduMotoCtrPra[Moto].MotoRunCmpValue;TIM_SetCompare2(TIM1, capture  );break;}
			 case MIDUMOTO3:{capture = TIM_GetCapture3(TIM1)+MiduMotoCtrPra[Moto].MotoRunCmpValue;TIM_SetCompare3(TIM1, capture  );break;}
                         case MIDUMOTO4:{capture = TIM_GetCapture4(TIM1)+MiduMotoCtrPra[Moto].MotoRunCmpValue;TIM_SetCompare4(TIM1, capture  );break;}
			 default:break;
                       	}
		   	    MiduMotoCtrPra[Moto].MotoDumuPuls--;
		   	}
   	   	}
	   else
	   	{
	   	    //结束进入半流 并关闭中断
	   	  
		   MiduMotoCtrPra[Moto].MotoRunState=MOTOSTOPHALF;
		   MiduMotoRefCtr(Moto,ENABLE);
		   switch(Moto)
		  	{
		  	  case MIDUMOTO1:{ TIM_ITConfig(TIM1, TIM_IT_CC1 , DISABLE);break;}
			  case MIDUMOTO2:{ TIM_ITConfig(TIM1, TIM_IT_CC2 , DISABLE);break;}
			  case MIDUMOTO3:{ TIM_ITConfig(TIM1, TIM_IT_CC3 , DISABLE);break;}
			  case MIDUMOTO4:{ TIM_ITConfig(TIM1, TIM_IT_CC4 , DISABLE);break;}
			  default:break;
		  	}
	   	}
   	}
}

void MotoDumuRun(uint8_t Moto,uint8_t Dir,uint16_t Plus,uint8_t Stop ,uint16_t HzMax)
{
   uint32_t  capture;
   MiduMotoCtrPra[Moto].MotoDumuPuls=Plus*2;
   if( MiduMotoCtrPra[Moto].MotoDumuPuls>0)
   {
       if(Dir==OUT_DIR_FAN)
       {
           MiduMotoCtrPra[Moto].MotoRunDir=OUT_DIR_FAN;
	       MiduMotoDirCtr(Moto,OUT_DIR_FAN);
       }
       else
       {
           MiduMotoCtrPra[Moto].MotoRunDir=OUT_DIR_ZHENG;
	       MiduMotoDirCtr(Moto,OUT_DIR_ZHENG);
   	   }
   }
   else
   {
      return;
   }

    //取得0位数据初始值
    MiduMotoCtrPra[Moto].MotoZeroState[2]=GetMiduMotoZero(Moto);
	MiduMotoCtrPra[Moto].MotoZeroState[1]=GetMiduMotoZero(Moto);
	MiduMotoCtrPra[Moto].MotoZeroState[0]=GetMiduMotoZero(Moto);
		
    MiduMotoStepCtr(Moto,OUT_DOWN);  
    GetMotoDumuStopData(Moto,Stop);    //设定停止数据
    //计算加速表
    MiduMotoCtrPra[Moto].MotoRunHzMax=HzMax;
    /*
    if(MiduMotoCtrPra[Moto].MotoRunHzMax<=MiduMotoCtrPra[Moto].MotoRunHzMin)
      	{ 
    	  MiduMotoCtrPra[Moto].MotoRunHzDangqian=MiduMotoCtrPra[Moto].MotoRunHzMax;
    	}
	else
		{
		  MiduMotoCtrPra[Moto].MotoRunHzDangqian=MiduMotoCtrPra[Moto].MotoRunHzMin;
		}
    */
    GetMotoAddTab(Moto,MiduMotoCtrPra[Moto].MotoRunHzMax);
	//设定第一次触发值
    MiduMotoCtrPra[Moto].MotoRunCmpValue=GetMotoCmpValue(MiduMotoCtrPra[Moto].MotoRunHzDangqian);
    MiduMotoRefCtr(Moto,DISABLE);
    MiduMotoFreeCtr(Moto,DISABLE);
    MiduMotoCtrPra[Moto].MotoRunState=MOTOWORKING;
	switch(Moto)
    	{
    	   case MIDUMOTO1:
		   	{
				capture=TIM_GetCounter(TIM1)+MiduMotoCtrPra[MIDUMOTO1].MotoRunCmpValue;
				TIM_SetCompare1(TIM1, capture);
				TIM_ClearFlag(TIM1, TIM_FLAG_CC1);
				TIM_ITConfig(TIM1, TIM_IT_CC1 , ENABLE);
				break;
    	   	        }
		   case MIDUMOTO2:
		   	{
				capture=TIM_GetCounter(TIM1)+MiduMotoCtrPra[MIDUMOTO2].MotoRunCmpValue;
				TIM_SetCompare2(TIM1, capture);
				TIM_ClearFlag(TIM1, TIM_FLAG_CC2);
				TIM_ITConfig(TIM1, TIM_IT_CC2 , ENABLE);
				break;
    	   	        }
		   case MIDUMOTO3:
		   	{
				capture=TIM_GetCounter(TIM1)+MiduMotoCtrPra[MIDUMOTO3].MotoRunCmpValue;
				TIM_SetCompare3(TIM1, capture);
				TIM_ClearFlag(TIM1, TIM_FLAG_CC3);
				TIM_ITConfig(TIM1, TIM_IT_CC3 , ENABLE);
				break;
    	   	        }
                    case MIDUMOTO4:
		   	{
				capture=TIM_GetCounter(TIM1)+MiduMotoCtrPra[MIDUMOTO4].MotoRunCmpValue;
				TIM_SetCompare4(TIM1, capture);
				TIM_ClearFlag(TIM1, TIM_FLAG_CC4);
				TIM_ITConfig(TIM1, TIM_IT_CC4 , ENABLE);
				break;
    	   	        }
		   default:
		   	{
				break;
		   	}
    	}

	
	
}
void Timer1ini(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;


  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  /*TIM1时钟配置*/
  TIM_TimeBaseStructure.TIM_Prescaler = DUMUCLKDIV-1;                      //6M计数频率
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //向上计数
  TIM_TimeBaseStructure.TIM_Period = 65535;                    //装载值选择最大
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
  TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing ;

  TIM_OC1Init(TIM1, &TIM_OCInitStructure);
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);
 // TIM_OC4Init(TIM1, &TIM_OCInitStructure);
  
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);
  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Disable);
  TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Disable);
    //TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Disable);

  TIM_ClearFlag(TIM1, TIM_FLAG_CC1); 
  TIM_ClearFlag(TIM1, TIM_FLAG_CC2); 
  TIM_ClearFlag(TIM1, TIM_FLAG_CC3); 
  TIM_ClearFlag(TIM1, TIM_FLAG_CC4); 


TIM_SetCounter(TIM1,0);


NVIC_InitStructure.NVIC_IRQChannel=TIM1_CC_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
NVIC_Init(&NVIC_InitStructure);

//TIM_ITConfig(TIM1, TIM_IT_CC1 , ENABLE); 
//TIM_ITConfig(TIM1, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3 , ENABLE);             //启动中断源
//TIM_ITConfig(TIM1, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4 , ENABLE);//启动中断源
TIM_Cmd(TIM1,ENABLE);


}
void MotoDuMuInit(void)
{
  uint8_t i;
  PinOutini();
  for(i=0;i<MiDuMotoNum;i++)
  	{
  	   MiduMotoCtrPra[i].MotoRunDir=OUT_DIR_ZHENG;
	   MiduMotoDirCtr(i,OUT_DIR_ZHENG);
	   MiduMotoRefCtr(i,ENABLE);
           MiduMotoFreeCtr(i,ENABLE);
	   MiduMotoCtrPra[i].MotoRunState=MOTOSTOPHALF;
	   MiduMotoCtrPra[i].MotoZeroState[2]=GetMiduMotoZero(i);
	   MiduMotoCtrPra[i].MotoZeroState[1]=GetMiduMotoZero(i);
	   MiduMotoCtrPra[i].MotoZeroState[0]=GetMiduMotoZero(i);
	   MiduMotoCtrPra[i].MotoDumuDangqian=0;
	   MiduMotoCtrPra[i].MotoDumuPuls=0;
	   MiduMotoCtrPra[i].MotoPwmPinState=0;
           GetMotoDumuStopData(i,0xff);          //忽略0位 运行。
	   MiduMotoCtrPra[i].MotoRunCmpValue=GetMotoCmpValue(MIDUMOTOHZMIN);
	   MiduMotoCtrPra[i].MotoRunHzDangqian=MIDUMOTOHZMIN;
           MiduMotoCtrPra[i].MotoRunHzMax=MIDUMOTOHZMIN;
           MiduMotoCtrPra[i].MotoRunHzMin=MIDUMOTOHZMIN;
	   GetMotoAddTab(i,MIDUMOTOHZMIN);
  	}
  Timer1ini();
}

void GetMotoAddValue(uint8_t  Moto)
{
  uint8_t  i;
  if(MiduMotoCtrPra[Moto].MotoDumuPuls<MotoSpeedAddTabNUM*2)
  	{
  	  //进入减速区
  	  if(MiduMotoCtrPra[Moto].MotoRunHzDangqian>MiduMotoCtrPra[Moto].MotoDumuAddTab[0])
  	  	{
  	  	   for(i=0;i<MotoSpeedAddTabNUM;i++)
  	  	   	{
  	  	   	  if(MiduMotoCtrPra[Moto].MotoRunHzDangqian>MiduMotoCtrPra[Moto].MotoDumuAddTab[MotoSpeedAddTabNUM-i-1])
  	  	   	  	{
  	  	   	  	  MiduMotoCtrPra[Moto].MotoRunHzDangqian=MiduMotoCtrPra[Moto].MotoDumuAddTab[MotoSpeedAddTabNUM-i-1];
				  return ;
  	  	   	  	}
  	  	   	}
  	  	}
  	}

  	else
  	{
  	  //进入加速区 
  	  if(MiduMotoCtrPra[Moto].MotoRunHzDangqian<MiduMotoCtrPra[Moto].MotoDumuAddTab[MotoSpeedAddTabNUM-1])
  	  	{
  	  	   for(i=0;i<MotoSpeedAddTabNUM;i++)
  	  	   	{
  	  	   	  if(MiduMotoCtrPra[Moto].MotoRunHzDangqian<MiduMotoCtrPra[Moto].MotoDumuAddTab[i])
  	  	   	  	{
  	  	   	  	   MiduMotoCtrPra[Moto].MotoRunHzDangqian=MiduMotoCtrPra[Moto].MotoDumuAddTab[i];
				   return ;
  	  	   	  	}
  	  	   	}
  	  	}
  	}
}
void GetMotoAddTab(uint8_t  Moto,uint16_t MaxHz)
{
   uint8_t  AddValue[MotoSpeedAddTabNUM];
   uint8_t  i;
   uint16_t  SumAddValue=0;
   uint16_t  HzChazhi;
    if(MiduMotoCtrPra[Moto].MotoRunHzMax<=MiduMotoCtrPra[Moto].MotoRunHzMin)
      	{ 
    	  MiduMotoCtrPra[Moto].MotoRunHzDangqian=MiduMotoCtrPra[Moto].MotoRunHzMax;
    	}
	else
	{
	  MiduMotoCtrPra[Moto].MotoRunHzDangqian=MiduMotoCtrPra[Moto].MotoRunHzMin;
	}
   
   if(MiduMotoCtrPra[Moto].MotoRunHzMax>MiduMotoCtrPra[Moto].MotoRunHzDangqian)
   	{
	   HzChazhi=  MiduMotoCtrPra[Moto].MotoRunHzMax-MiduMotoCtrPra[Moto].MotoRunHzDangqian;
	   for(i=0;i<MotoSpeedAddTabNUM/2;i++)
	   	{
	   	   AddValue[i]=i;
		   AddValue[MotoSpeedAddTabNUM-i-1]=i;
	           SumAddValue=SumAddValue+AddValue[i]+AddValue[MotoSpeedAddTabNUM-i-1];
	   	}
	    AddValue[MotoSpeedAddTabNUM/2]=MotoSpeedAddTabNUM/2;
	    SumAddValue=SumAddValue+AddValue[MotoSpeedAddTabNUM/2];
		MiduMotoCtrPra[Moto].MotoDumuAddTab[0]=MiduMotoCtrPra[Moto].MotoRunHzDangqian;
	   for(i=1;i<MotoSpeedAddTabNUM;i++)
	   {
	       MiduMotoCtrPra[Moto].MotoDumuAddTab[i]=MiduMotoCtrPra[Moto].MotoDumuAddTab[i-1]+ (HzChazhi*AddValue[i])/SumAddValue;
	   }
           MiduMotoCtrPra[Moto].MotoDumuAddTab[MotoSpeedAddTabNUM-1]=MaxHz;
           
   	}
    else
    {
      for(i=0;i<MotoSpeedAddTabNUM;i++)
	   {
	       MiduMotoCtrPra[Moto].MotoDumuAddTab[i]=MiduMotoCtrPra[Moto].MotoRunHzDangqian;
	   }
    }
   
  
}
uint32_t GetMotoCmpValue(uint16_t MotorunHz)
{
  uint32_t buf;
  if(MotorunHz<DUMUMOTOSTOPHZ)
  	{
  	  MotorunHz=DUMUMOTOSTOPHZ;
  	}
  buf=DUMUCLKDIV*2;
  buf=SystemCoreClock/buf;
  buf=buf/MotorunHz;
  	
  return buf;
}

void PinOutini(void)
{
  uint8_t i;
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB , ENABLE);
   //output
   for(i=0;i<MiDuMotoNum;i++)
   	{    
      GPIO_InitStructure.GPIO_Pin = MiduMotoRef[i].bit;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
      GPIO_Init(MiduMotoRef[i].port, &GPIO_InitStructure);
	  GPIO_InitStructure.GPIO_Pin = MiduMotoFree[i].bit;
	  GPIO_Init(MiduMotoFree[i].port, &GPIO_InitStructure);
	  GPIO_InitStructure.GPIO_Pin = MiduMotoStep[i].bit;
	  GPIO_Init(MiduMotoStep[i].port, &GPIO_InitStructure);
	  GPIO_InitStructure.GPIO_Pin = MiduMotoDir[i].bit;
	  GPIO_Init(MiduMotoDir[i].port, &GPIO_InitStructure);
	 
   }

    //input
   
   for(i=0;i<MiDuMotoNum;i++){
      GPIO_InitStructure.GPIO_Pin = MiduMotoZero[i].bit;	
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
      GPIO_Init(MiduMotoZero[i].port, &GPIO_InitStructure);
   }
}

void MiduMotoRefCtr(uint8_t Moto,uint8_t NewState)
{
   if (NewState == ENABLE)
   	{
   	  GPIO_ResetBits(MiduMotoRef[Moto].port,MiduMotoRef[Moto].bit);
   	}
   else
   	{
   	   GPIO_SetBits(MiduMotoRef[Moto].port,MiduMotoRef[Moto].bit);
   	}
}

void MiduMotoFreeCtr(uint8_t Moto,uint8_t NewState)
{
   if (NewState == DISABLE)
   	{
   	  GPIO_ResetBits(MiduMotoFree[Moto].port,MiduMotoFree[Moto].bit);
   	}
   else
   	{
   	   GPIO_SetBits(MiduMotoFree[Moto].port,MiduMotoFree[Moto].bit);
   	}
}

void MiduMotoDirCtr(uint8_t Moto,uint8_t NewState)
{
   if (NewState == OUT_DIR_FAN )
   	{
   	  GPIO_ResetBits(MiduMotoDir[Moto].port,MiduMotoDir[Moto].bit);
   	}
   else
   	{
   	   GPIO_SetBits(MiduMotoDir[Moto].port,MiduMotoDir[Moto].bit);
   	}
}
void MiduMotoStepCtr(uint8_t Moto,uint8_t NewState)
{
   if (NewState == OUT_DOWN )
   	{
   	  GPIO_ResetBits(MiduMotoStep[Moto].port,MiduMotoStep[Moto].bit);
   	}
   else
   	{
   	   GPIO_SetBits(MiduMotoStep[Moto].port,MiduMotoStep[Moto].bit);
   	}
}
uint8_t GetMiduMotoZero(uint8_t Moto)
{
  
   return (GPIO_ReadInputDataBit(MiduMotoZero[Moto].port,MiduMotoZero[Moto].bit));
}




void GetMotoDumuStopData(uint8_t Moto,uint8_t  Stop)
{
   uint8_t  i;
   for(i=0;i<MiDuMotoNum;i++)
   	{
   	    MiduMotoCtrPra[Moto].MotoStopData[i]=(Stop>>(i*2));
		MiduMotoCtrPra[Moto].MotoStopData[i]&=0x03;
   	}
}