#include "rtc.h"
#include "delay.h"
#include "common.h"
#include <string.h>
#include <stdlib.h>


typedef struct ReflashTime{

	BOOL_E(*StepFun)(void);
	
	struct ReflashTime* NextStep;
}ReflashTimeStep_S;

static struct {
	Date_S Date;
	ReflashTimeStep_S *ReflashTimeNode;

}RtcObj_S;


static void Rtc_ReflashTime(void)
{
	if(RtcObj_S.ReflashTimeNode == NULL) return;
	
	ReflashTimeStep_S* ReflashTimeNode_t;
	ReflashTimeNode_t = RtcObj_S.ReflashTimeNode;
	
	while(1)
	{		
		if(ReflashTimeNode_t->StepFun() != TRUE)
		{
			if(ReflashTimeNode_t->NextStep == NULL) break;
			else ReflashTimeNode_t = ReflashTimeNode_t->NextStep;
		}
		else break;
	}
}

//向前增加节点
static BOOL_E Rtc_RegReflashTimeNode(BOOL_E(*StepFun)(void))
{
	ReflashTimeStep_S *ReflashTimeNode_t;
	
	if(StepFun == NULL) return FALSE;
	
	ReflashTimeNode_t = malloc(sizeof(ReflashTimeStep_S));
	if(ReflashTimeNode_t == NULL) return FALSE;
	
	ReflashTimeNode_t->StepFun = StepFun;
	ReflashTimeNode_t->NextStep = RtcObj_S.ReflashTimeNode;
	RtcObj_S.ReflashTimeNode = ReflashTimeNode_t;
	
	return TRUE;
	
} 

static BOOL_E IncSec(void)
{
	if(RtcObj_S.Date.Sec < 59) {RtcObj_S.Date.Sec++; return TRUE;}
	else {RtcObj_S.Date.Sec = 0; return FALSE;}
}	

static BOOL_E IncMin(void)
{
	if(RtcObj_S.Date.Min < 59) {RtcObj_S.Date.Min++; return TRUE;}
	else {RtcObj_S.Date.Min = 0; return FALSE;}
}	

static BOOL_E IncHour(void)
{
	if(RtcObj_S.Date.Hour < 23) {RtcObj_S.Date.Hour++; return TRUE;}
	else {RtcObj_S.Date.Hour = 0; return FALSE;}
}

static BOOL_E Is_Leap_Year(u16 year)
{			  
	if(year%4==0) //必须能被4整除
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return TRUE;//如果以00结尾,还要能被400整除 	   
			else return FALSE;   
		}else return TRUE;   
	}else return FALSE;	
}	 

u8 MonTab[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表	 

static Week_E RTC_Get_Week(Date_S const *iDate)
{	
	u16 temp2;
	u8 yearH,yearL;
	
	yearH=iDate->Year/100;	yearL=iDate->Year%100; 
	// 如果为21世纪,年份数加100  
	if (yearH>19)yearL+=100;
	// 所过闰年数只算1900年之后的  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+iDate->Day+table_week[iDate->Month-1];
	if (yearL%4==0&&iDate->Month<3)temp2--;
	return(Week_E)(temp2%7);
}

static BOOL_E IncDay(void)
{
	
	if(Is_Leap_Year(RtcObj_S.Date.Year)) MonTab[1] = 29;
	
	if(RtcObj_S.Date.Day < MonTab[RtcObj_S.Date.Month]) {RtcObj_S.Date.Day++; return TRUE;}
	else {RtcObj_S.Date.Day = 1; return FALSE;}
}

static BOOL_E IncMonth(void)
{
	if(RtcObj_S.Date.Month < 12) {RtcObj_S.Date.Month++; return TRUE;}
	else {RtcObj_S.Date.Month = 1; return FALSE;}
}

static BOOL_E IncYear(void)
{
	RtcObj_S.Date.Year++;
	return TRUE;
} 

static u32 DateToSec(Date_S const *iDate)
{
	u16 t;
	u32 seccount=0;
	//u8 Month_t = iDate->Month;
	//if(iDate->Year<1970||iDate->Year>2099)return 1;	   
	for(t=1970;t<iDate->Year;t++)	//把所有年份的秒钟相加
	{
		if(Is_Leap_Year(t))seccount+=31622400;//闰年的秒钟数
		else seccount+=31536000;			  //平年的秒钟数
	}

	for(t=0;t<(iDate->Month - 1);t++)	   //把前面月份的秒钟数相加
	{
		seccount+=(u32)MonTab[t]*86400;//月份秒钟数相加
		if(Is_Leap_Year(iDate->Year)&&t==1)seccount+=86400;//闰年2月份增加一天的秒钟数	   
	}
	seccount+=(u32)(iDate->Day-1)*86400;//把前面日期的秒钟数相加 
	seccount+=(u32)iDate->Hour*3600;//小时秒钟数
  seccount+=(u32)iDate->Min*60;	 //分钟秒钟数
	seccount+=iDate->Sec;//最后的秒钟加上去
	
	return seccount;
}

static void SecToDate(Date_S *oDate, u32 Sec)
{ 
	u32 temp=0;
	u16 temp1=0;
	
		temp=Sec/86400;   //得到天数(秒钟数对应的)
		temp1=1970;	//从1970年开始
		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))//是闰年
			{
				if(temp>=366)temp-=366;//闰年的秒钟数
				else {temp1++;break;}  
			}
			else temp-=365;	  //平年 
			temp1++;  
		}   
		oDate->Year=temp1;//得到年份
		temp1=0;
		while(temp>=28)//超过了一个月
		{
			if(Is_Leap_Year(oDate->Year)&&temp1==1)//当年是不是闰年/2月份
			{
				if(temp>=29)temp-=29;//闰年的秒钟数
				else break; 
			}
			else 
			{
				if(temp>=MonTab[temp1])temp-=MonTab[temp1];//平年
				else break;
			}
			temp1++;  
		}
		oDate->Month=temp1+1;	//得到月份
		oDate->Day=temp+1;  	//得到日期 

	temp=Sec%86400;     		//得到秒钟数   	   
	oDate->Hour=temp/3600;     	//小时
	oDate->Min=(temp%3600)/60; 	//分钟	
	oDate->Sec=(temp%3600)%60; 	//秒钟
	oDate->Week=RTC_Get_Week(oDate);//获取星期   
}

//注册顺序有要求
static void DateInit(void)
{
	Rtc_RegReflashTimeNode(IncYear);
	Rtc_RegReflashTimeNode(IncMonth);
	Rtc_RegReflashTimeNode(IncDay);
	Rtc_RegReflashTimeNode(IncHour);
	Rtc_RegReflashTimeNode(IncMin);
	Rtc_RegReflashTimeNode(IncSec);
}

static void RTC_NVIC_Config(void)
{	
  NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTC全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//先占优先级1位,从优先级3位
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//先占优先级0位,从优先级4位
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//使能该通道中断
	NVIC_Init(&NVIC_InitStructure);		//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}


void Rtc_InitDate(void)
{
   SecToDate(&RtcObj_S.Date, RTC_GetCounter());
}	 

void Rtc_Init(void)
{
	u8 temp=0;
	Date_S Date;
	Date.Year = 2020;
	Date.Month = 11;
	Date.Day = 1;
	Date.Hour = 12;
	Date.Min = 22;
	Date.Sec = 0;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
	PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问  
	
	if (BKP_ReadBackupRegister(BKP_DR1) != 0x5050)		//从指定的后备寄存器中读出数据:读出了与写入的指定数据不相乎
	{	 			
		BKP_DeInit();	//复位备份区域 	
		RCC_LSEConfig(RCC_LSE_ON);	//设置外部低速晶振(LSE),使用外设低速晶振
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET&&temp<250)	//检查指定的RCC标志位设置与否,等待低速晶振就绪
			{
			temp++;
			delay_ms(10);
			}
		//if(temp>=250)return 1;//初始化时钟失败,晶振有问题	    
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//设置RTC时钟(RTCCLK),选择LSE作为RTC时钟    
		RCC_RTCCLKCmd(ENABLE);	//使能RTC时钟  
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		RTC_WaitForSynchro();		//等待RTC寄存器同步  
		RTC_ITConfig(RTC_IT_SEC, ENABLE);		//使能RTC秒中断
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		RTC_EnterConfigMode();/// 允许配置	
		RTC_SetPrescaler(32767); //设置RTC预分频的值
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		//RTC_Set(2015,1,14,17,42,55);  //设置时间	
		Rtc_SetDate(&Date);
		RTC_ExitConfigMode(); //退出配置模式  
		BKP_WriteBackupRegister(BKP_DR1, 0X5050);	//向指定的后备寄存器中写入用户程序数据
		PWR_BackupAccessCmd(DISABLE);
	}
	else//系统继续计时
	{

		RTC_WaitForSynchro();	//等待最近一次对RTC寄存器的写操作完成
		RTC_ITConfig(RTC_IT_SEC, ENABLE);	//使能RTC秒中断
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
	}
	DateInit();
		
	Rtc_InitDate();
	RTC_NVIC_Config();//RCT中断分组设置		    				     
	//return 0; //ok	 				    
}

void RTC_IRQHandler(void)
{		 
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)//秒钟中断
	{					
		Rtc_ReflashTime();
		RTC_ClearITPendingBit(RTC_IT_SEC);
 	}				  								 	  	    						 	   	 
}



void Rtc_SetDate(Date_S const *iDate)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
	PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问  
	
	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
	RTC_EnterConfigMode();/// 允许配置
	RTC_SetCounter(DateToSec(iDate));	//设置RTC计数器的值

	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成  	
	PWR_BackupAccessCmd(DISABLE);
		
}

void Rtc_GetDate(Date_S *oDate)
{
	memcpy(oDate, &RtcObj_S.Date, sizeof(Date_S));
}

