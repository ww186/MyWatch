#ifndef _COMMON_H_
#define _COMMON_H_
#include "sys.h"
typedef enum
{
	FALSE = 0,
	TRUE,
}BOOL_E;

typedef enum
{
	Sun = 0,
	Mon,
	Tue,
	Wed,
	Thur,
	Fri,
	Sat,
}Week_E;

typedef struct{
	u16 Year;
	u8 Month;
	u8 Day;
	u8 Hour;
	u8 Min;
	u8 Sec;
	Week_E Week;
}Date_S;



#endif



