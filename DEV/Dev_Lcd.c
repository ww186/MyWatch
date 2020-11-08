#include "lcd.h"
#include "stdlib.h"
#include "usart.h"	 
#include "delay.h"
#include "Dev_Lcd.h"
#include "Config.h"


LCD_S *LcdDev;

void App_DisplayInit(void)
{
	//LcdDev = &LCD_DEV;
	if(LcdDev != NULL) {if(LcdDev->init != NULL) LcdDev->init();}
}

void App_DisplayTime(const Date_S* Date)
{
	if(LcdDev != NULL) {if(LcdDev->ShowDate != NULL)LcdDev->ShowDate(Date);}
}

void App_Clear(void)
{
	if(LcdDev != NULL) {if(LcdDev->Clear != NULL) LcdDev->Clear();}
}


