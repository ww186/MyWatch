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
#include "timer.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "demo.h"


static void InitLvTheme(void)
{
	lv_theme_t* theme = lv_theme_night_init(210, NULL);
	lv_theme_set_current(theme);
}
 	
int main(void)
{	 
	//Date_S Date;
	
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);
	TIM6_Int_Init(10);
	lv_init();
	lv_port_disp_init();
	InitLvTheme();
	KEY_Init();
	//App_DisplayInit();
	Rtc_Init();
	Page_Init();

	
	//demo_create();
	
	while(1)
	{
		if(Is10MsTick())
		{
			lv_tick_inc(10);
			KEY_ScanProcess();
			
			//Page_RefalshProcess();
		}
		
		lv_task_handler();
		
	}
	
	 
}
