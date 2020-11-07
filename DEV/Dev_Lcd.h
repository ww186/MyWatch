#ifndef __DEV_LCD_H
#define __DEV_LCD_H		
#include "sys.h"	 
#include "stdlib.h"
#include "common.h"


typedef struct{
	void (*init)(void);
	void (* ShowDate)(const Date_S* Date);
	void (*Clear)(void);
}LCD_S;

void App_DisplayInit(void);
void App_DisplayTime(const Date_S* Date);
void App_Clear(void);

#endif
