#ifndef _COMMON_H_
#define _COMMON_H_
#include "sys.h"
#include "lvgl.h"
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

void lv_AddAnimObj(lv_obj_t* obj, lv_coord_t Start_y, lv_coord_t End_y, lv_anim_exec_xcb_t exec_cb, uint16_t time);

#endif



