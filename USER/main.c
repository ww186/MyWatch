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
#include "Dev_ExFlash.h"
#include "spi.h"
#include "dma.h"
#include "ff.h" 
#include "adc.h"


static void InitLvTheme(void)
{
	lv_theme_t* theme = lv_theme_night_init(210, NULL);
	lv_theme_set_current(theme);
}



int main(void)
{	 
	lv_fs_file_t file;
	u32 br, bw;
	u8 buffer[100];
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);
	SpiInit();
	InitDma();
	Adc_Init();
	
	TimerTaskInit();
	lv_init();
	
	lv_port_disp_init();
	InitLvTheme();
	KEY_Init();
	Rtc_Init();
	Page_Init();
	lv_task_create((lv_task_cb_t)KEY_ScanProcess, 10, LV_TASK_PRIO_HIGH, NULL);
	TimerTaskReg(lv_tick_inc, 10, 10);
	FatFsInit();

//	if(lv_fs_open(&file, "1:a.txt", LV_FS_MODE_OPEN_ALWAYS | LV_FS_MODE_WR ) == LV_FS_RES_OK) 
//	{
//		printf("open sucess\r\n");
//		if(lv_fs_write(&file, "LV Test3 Ok!\r\n", 14, &br) == LV_FS_RES_OK) printf("write sucess\r\n");

//		lv_fs_close(&file);

		if(lv_fs_open(&file, "1:a.txt", LV_FS_MODE_OPEN_EXISTING | LV_FS_MODE_RD ) == LV_FS_RES_OK) 
		{
			printf("open2 sucess\r\n");
			if(lv_fs_read(&file, buffer, 100, &bw) == LV_FS_RES_OK) 
			{
				printf("%s\r\n", buffer);

			}

			lv_fs_close(&file);

		}
//	}
//	

	while(1)
	{
		lv_task_handler();
	}
	
	 
}
