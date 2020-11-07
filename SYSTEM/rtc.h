#ifndef _RTC_H
#define _RTC_H

#include "sys.h"
#include "common.h"


void Rtc_Init(void);
void Rtc_GetDate(Date_S *oDate);
void Rtc_SetDate(Date_S const *iDate);
void Rtc_InitDate(void);     //通过RTC计数器计算更新时间


#endif 




