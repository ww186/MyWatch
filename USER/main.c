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


static void InitLvTheme(void)
{
	lv_theme_t* theme = lv_theme_night_init(210, NULL);
	lv_theme_set_current(theme);
}
 	 u8 TEXT_Buffer[]={"WarShipSTM32 SPI TEST"};
	u8 Rx_Buffer[50];
int main(void)
{	 
	u8 i;
	FIL fil;
	FATFS fs;
	FRESULT res;
	UINT bw, br;
	BYTE work[FF_MAX_SS];
	BYTE buffer[100];
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);
	SpiInit();
	//InitW25qxx();
	InitDma();
	
//	res = f_mkfs("1:", 0, work, sizeof(work));
//	
//	if(res == 0)
//	{
//		printf("Mkfs Sucess\r\n");
//		res = f_mount(&fs,"1:",1);
//		if(res == 0) printf("Mount Sucess\r\n");
//	}
	
	res = f_mount(&fs,"1:",1);
		if(res == 0) printf("Mount Sucess\r\n");
	
	res = f_open(&fil, "1:a.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
	printf("res %d\r\n", res);
	
	if(res == 0)
	{
		res = f_write(&fil, "Hello, World!\r\n", 15, &bw);
		f_close(&fil);
		//f_lseek(&fil, 0);
		res = f_open(&fil, "1:a.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
		f_lseek(&fil, 0);
		if(res == 0)
		{
			 res = f_read(&fil, buffer, sizeof(buffer), &br);
				printf("read res %d\r\n", res);
				printf("br %d\r\n", br);
			  if(res == 0) printf("%s\r\n", buffer);
		}
		
		f_close(&fil);
		
	}
	
	
//	W25QXX_Write_Page(TEXT_Buffer, 200, sizeof(TEXT_Buffer));
//	W25QXX_Read(Rx_Buffer, 200, sizeof(TEXT_Buffer));
//	for(i=0; i<sizeof(TEXT_Buffer); i++)
//	{
//		printf("%c", Rx_Buffer[i]);
//	}
//	printf("__FlashId %x\r\n", W25QXX_ReadID());
//	TimerTaskInit();
//	lv_init();
//	lv_port_disp_init();
//	InitLvTheme();
//	KEY_Init();
//	Rtc_Init();
//	Page_Init();
//	lv_task_create((lv_task_cb_t)KEY_ScanProcess, 10, LV_TASK_PRIO_HIGH, NULL);
//	TimerTaskReg(lv_tick_inc, 10, 10);
	
	while(1)
	{
//		lv_task_handler();
	}
	
	 
}
