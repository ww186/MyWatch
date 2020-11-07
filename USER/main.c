#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "Dev_Lcd.h"
#include "rtc.h"
#include "common.h"
#include "Page.h"
 	
int main(void)
{	 
	//Date_S Date;
	
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);
	KEY_Init();
	App_DisplayInit();
	Rtc_Init();
	Page_Init();
	
	while(1)
	{
		KEY_ScanProcess();
		Page_RefalshProcess();
		
		delay_ms(10);
	}
	
	 
}
