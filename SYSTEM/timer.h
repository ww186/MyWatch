#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
#include "common.h"

typedef void (*TimerTask)(u32 data);

void TimerTaskInit(void);
BOOL_E TimerTaskReg(TimerTask iTask, u32 iPeriod, u32 iData);



#endif


