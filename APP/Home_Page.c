#include "Home_Page.h"
#include "Dev_Lcd.h"
#include "rtc.h"
#include "usart.h"
#include "lvgl.h"	
LV_FONT_DECLARE(font_hei48);

PageNode_S* ChildNodeList[] = {
    &ManagePageNode,
};

static Date_S Date;

static u8* Week[] = {"Sun", "Mon", "Tue", "Wed", "Thur", "Fri", "Sat"};

//year-month-day 
static lv_obj_t* DateLabGrp[10];

static lv_obj_t* LabWeek;

//Hour Min Sec
static lv_obj_t* TimeLabGrp[6];
static lv_obj_t* TimeLabGrp_t[6];   //动态显示时间时需用到

static lv_obj_t* TimeLed1;
static lv_obj_t* TimeLed2;

static lv_obj_t* LabBat;

static lv_obj_t* ContForWholePage;
static lv_obj_t* ContDate;
static lv_obj_t* ContTop;
static lv_obj_t* ContTime;

static lv_task_t* taskTimeUpdate;



static void DateLabCreate(lv_obj_t* cont)
{
    u8 i;

    for(i=0; i<sizeof(DateLabGrp)/sizeof(lv_obj_t*); i++)
    {
        DateLabGrp[i] = lv_label_create(cont, NULL);
        lv_label_set_text(DateLabGrp[i], "-");
        /*
        if((i == 4) || (i == 7))
            lv_label_set_text(DateLabGrp[i], "-");
        else
        {
           lv_label_set_text(DateLabGrp[i], "0");
        }
        */
    }
        
}

static void TimeLabCreate(lv_obj_t* cont)
{
    u8 i;
    static lv_style_t DateLabStyle;

    lv_style_copy(&DateLabStyle, &lv_style_plain_color);
    DateLabStyle.text.font = &font_hei48;
    DateLabStyle.text.color = LV_COLOR_WHITE;

    for(i=0; i<sizeof(TimeLabGrp)/sizeof(lv_obj_t*); i++)
    {
        TimeLabGrp[i] = lv_label_create(cont, NULL);
        lv_label_set_style(TimeLabGrp[i], LV_CONT_STYLE_MAIN, &DateLabStyle);
        lv_label_set_text(TimeLabGrp[i], "-");
        if(i<2)
            lv_obj_align(TimeLabGrp[i], NULL, LV_ALIGN_IN_TOP_MID,  -((3-i)*lv_obj_get_width(TimeLabGrp[i])), 0);
        else
            lv_obj_align(TimeLabGrp[i], NULL, LV_ALIGN_IN_TOP_MID,  -((3-i)*lv_obj_get_width(TimeLabGrp[i]))+16, 0);
    }

    for(i=0; i<sizeof(TimeLabGrp_t)/sizeof(lv_obj_t*); i++)
    {
        TimeLabGrp_t[i] = lv_label_create(cont, NULL);
        lv_label_set_style(TimeLabGrp_t[i], LV_CONT_STYLE_MAIN, &DateLabStyle);
        lv_label_set_text(TimeLabGrp_t[i], "0");
        lv_obj_align(TimeLabGrp_t[i], TimeLabGrp[i], LV_ALIGN_OUT_BOTTOM_MID,  0, 10);
    }

   

}

static void TimeLedCreate(lv_obj_t* cont)
{
    static lv_style_t led_style;

    led_style = lv_style_pretty_color;
    led_style.body.main_color = LV_COLOR_RED;
    led_style.body.grad_color = LV_COLOR_RED;

    TimeLed1 = lv_led_create(cont, NULL);
    TimeLed2 = lv_led_create(cont, NULL);

    lv_led_set_style(TimeLed1, LV_LED_STYLE_MAIN, &led_style);
    lv_led_set_style(TimeLed2, LV_LED_STYLE_MAIN, &led_style);
    lv_obj_set_size(TimeLed1, 8, 10);
    lv_obj_set_size(TimeLed2, 8, 10);

    lv_obj_align(TimeLed1, NULL, LV_ALIGN_OUT_TOP_MID, -3*lv_obj_get_width(TimeLabGrp[0])/2+8, lv_obj_get_height(TimeLabGrp[0])/2);
    lv_obj_align(TimeLed2, TimeLed1, LV_ALIGN_OUT_TOP_MID, 0, 25);
}


static void TopContCreate(void)
{
    ContTop = lv_cont_create(ContForWholePage, NULL);
    ContDate = lv_cont_create(ContTop, NULL);

    DateLabCreate(ContDate);

    LabWeek = lv_label_create(ContTop, NULL);
    LabBat = lv_label_create(ContTop, NULL);

    lv_label_set_text(LabWeek, "---");
    lv_label_set_text(LabBat, LV_SYMBOL_BATTERY_FULL);

    lv_cont_set_fit2(ContTop, LV_FIT_FLOOD, LV_FIT_TIGHT);
    lv_cont_set_layout(ContTop, LV_LAYOUT_OFF);
    lv_cont_set_style(ContTop, LV_CONT_STYLE_MAIN, &lv_style_transp);

    lv_cont_set_layout(ContDate, LV_LAYOUT_ROW_M);
    lv_cont_set_fit(ContDate, LV_FIT_TIGHT);
    lv_cont_set_style(ContDate, LV_CONT_STYLE_MAIN, &lv_style_transp);
    lv_obj_align(ContDate, ContTop, LV_ALIGN_IN_TOP_LEFT, 0, 0);

    lv_obj_align(LabWeek, NULL, LV_ALIGN_IN_TOP_RIGHT, -42, 6);
    lv_obj_align(LabBat, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 6);
}

static void TimeContCreate(void)
{
    ContTime = lv_cont_create(ContForWholePage, NULL);

    lv_cont_set_layout(ContTime, LV_LAYOUT_OFF);
    lv_cont_set_fit(ContTime, LV_FIT_NONE);
    lv_obj_set_size(ContTime, lv_obj_get_width(ContForWholePage), font_hei48.line_height);
    lv_cont_set_style(ContTime, LV_CONT_STYLE_MAIN, &lv_style_transp);

    TimeLabCreate(ContTime);
    TimeLedCreate(ContTime);
}

static void PageContCreate(void)
{
    lv_obj_t * scr = lv_scr_act();
    ContForWholePage = lv_page_create(scr, NULL);
    
    lv_page_set_scrl_layout(ContForWholePage, LV_LAYOUT_COL_M);

    lv_page_set_sb_mode(ContForWholePage, LV_SB_MODE_OFF);
	lv_obj_set_size(ContForWholePage, 200, 120);
	lv_obj_align(ContForWholePage, NULL, LV_ALIGN_CENTER, 0, 0);
}



static void CreatShowTimeUi(void)
{
    
    TopContCreate();
    TimeContCreate();

    
}

void SetHomePageInitDate(void)
{
    Date.Year = 0xffff;
    Date.Month = 0xff;
    Date.Day = 0xff;
    Date.Week = Week_NONE;
    Date.Hour = 0xff;
    Date.Min = 0xff;
    Date.Sec = 0xff;
}



static void lv_anim_exec_cb_num(lv_obj_t* Lab, lv_anim_value_t coord)
{
    printf("__lv_anim_exec_cb_num Coord %d \r\n", coord);
    lv_obj_set_y(Lab, coord);
}

static void lv_anim_Ready_cb_num(lv_anim_t* a)
{
    printf("__lv_anim_Ready_cb_num \r\n");
    //lv_label_set_text(TimeLabGrp[0], "1");
}

static void DisplayLabAnim(lv_obj_t* Lab, lv_coord_t Start_y, lv_coord_t End_y, uint16_t time)
{
    lv_coord_t AnimStart_y, AnimEnd_y;
    lv_anim_t a;
    //AnimStart_y = lv_obj_get_y(Lab);
    AnimStart_y = Start_y;
    AnimEnd_y = End_y;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, Lab, (lv_anim_exec_xcb_t)lv_anim_exec_cb_num);
    //lv_anim_set_ready_cb(&a, lv_anim_Ready_cb_num );
    lv_anim_set_values(&a, AnimStart_y, AnimEnd_y);
    lv_anim_set_path_cb(&a, lv_anim_path_linear);
    lv_anim_set_time(&a, time, 0);

    lv_anim_create(&a);
}

static void LabelSetNum(lv_obj_t* Lab, u8 Num)
{
     switch(Num)
    {
        case 0: lv_label_set_text(Lab, "0"); break;

        case 1: lv_label_set_text(Lab, "1"); break;

        case 2: lv_label_set_text(Lab, "2"); break;

        case 3: lv_label_set_text(Lab, "3"); break;

        case 4: lv_label_set_text(Lab, "4"); break;

        case 5: lv_label_set_text(Lab, "5"); break;

        case 6: lv_label_set_text(Lab, "6"); break;

        case 7: lv_label_set_text(Lab, "7"); break;

        case 8: lv_label_set_text(Lab, "8"); break;

        case 9: lv_label_set_text(Lab, "9"); break;

        default: break;

    }
}

static void DisplayTimeAnim(u8 TimeGrpIndex, u8 NewValue)
{
    lv_obj_t * next_label;
    lv_obj_t * now_label;
    lv_coord_t Offset_y; 

    if(lv_obj_get_y(TimeLabGrp[TimeGrpIndex]) > lv_obj_get_y(TimeLabGrp_t[TimeGrpIndex]))
    {
        
        now_label = TimeLabGrp_t[TimeGrpIndex];
        next_label = TimeLabGrp[TimeGrpIndex];
    }
    else
    {
       next_label = TimeLabGrp_t[TimeGrpIndex];
       now_label = TimeLabGrp[TimeGrpIndex];
    }

    LabelSetNum(next_label, NewValue);

    lv_obj_align(next_label, now_label, LV_ALIGN_OUT_TOP_MID,  0, -10);

    Offset_y = lv_obj_get_y(now_label) - lv_obj_get_y(next_label);

    DisplayLabAnim(now_label, lv_obj_get_y(now_label), lv_obj_get_y(now_label)+Offset_y, LV_ANIM_TIME_DEFAULT);
    DisplayLabAnim(next_label, lv_obj_get_y(next_label), lv_obj_get_y(next_label)+Offset_y, LV_ANIM_TIME_DEFAULT);

}

static void DisplayNumStatic(lv_obj_t* Lab, u8 Num)
{
     LabelSetNum(Lab, Num);
}

static void DisplayWeek(Week_E iWeek)
{
    lv_label_set_text(LabWeek, (char*)Week[iWeek]);

}

static void DisplayYear(u16 Year)
{
    if(Date.Year/1000 != Year/1000)
        DisplayNumStatic(DateLabGrp[0], Year/1000);
    if(Date.Year%1000/100 != Year%1000/100)
        DisplayNumStatic(DateLabGrp[1], Year%1000/100);
    if(Date.Year%1000%100/10 != Year%1000%100/10)
        DisplayNumStatic(DateLabGrp[2], Year%1000%100/10);
    if(Date.Year%1000%100%10 != Year%1000%100%10)
        DisplayNumStatic(DateLabGrp[3], Year%1000%100%10);
}

static void DisplayMonth(u8 Month)
{
    if(Date.Month/10 != Month/10)
        DisplayNumStatic(DateLabGrp[5], Month/10);
    if(Date.Month%10 != Month%10)
        DisplayNumStatic(DateLabGrp[6], Month%10);
}

static void DisplayDay(u8 Day)
{
    if(Date.Day/10 != Day/10)
        DisplayNumStatic(DateLabGrp[8], Day/10);
    if(Date.Day%10 != Day%10)
        DisplayNumStatic(DateLabGrp[9], Day%10);
}

static void DisplayHour(u8 Hour)
{
    if(Date.Hour/10 != Hour/10)
        DisplayTimeAnim(0, Hour/10);
    if(Date.Hour%10 != Hour%10)
        DisplayTimeAnim(1, Hour%10);
}

static void DisplayMin(u8 Min)
{
   if(Date.Min/10 != Min/10)
        DisplayTimeAnim(2, Min/10);
    if(Date.Min%10 != Min%10)
        DisplayTimeAnim(3, Min%10);
}

static void DisplaySec(u8 Sec)
{
   if(Date.Sec/10 != Sec/10)
        DisplayTimeAnim(4, Sec/10);
    if(Date.Sec%10 != Sec%10)
        DisplayTimeAnim(5, Sec%10);
}

static void DisplayDate(Date_S* const iDate)
{
	
    if(Date.Year != iDate->Year)
    {
        DisplayYear(iDate->Year);
        Date.Year = iDate->Year;
    }

    if(Date.Month != iDate->Month)
	{
		DisplayMonth(iDate->Month);
		Date.Month = iDate->Month;
	}

    if(Date.Day != iDate->Day)
	{
		DisplayDay(iDate->Day);
		Date.Day = iDate->Day;
	}


    if(Date.Week != iDate->Week)
    {
        DisplayWeek(iDate->Week);
        Date.Week = iDate->Week;
    }
   

    if(Date.Hour != iDate->Hour)
	{
		DisplayHour(iDate->Hour);
		Date.Hour = iDate->Hour;
	}
	
	if(Date.Min != iDate->Min)
	{
		DisplayMin(iDate->Min);
		Date.Min = iDate->Min;
	}
	
	if(Date.Sec != iDate->Sec)
	{
		DisplaySec(iDate->Sec);
		Date.Sec = iDate->Sec;
	}
}

static void ReflashHomePage(void)
{
    Date_S Date_t;
    lv_led_toggle(TimeLed1);
    lv_led_toggle(TimeLed2);
    Rtc_GetDate(&Date_t);
    DisplayDate(&Date_t);
}

static void OpenHomePage(void)
{
    lv_obj_move_foreground(ContForWholePage);
    CreatShowTimeUi();
    Rtc_InitDate();
    taskTimeUpdate = lv_task_create((lv_task_cb_t)ReflashHomePage, 500, LV_TASK_PRIO_MID, NULL);
    lv_task_ready(taskTimeUpdate);
    printf("__Open Home Page\r\n");
}

static void CloseHomePage(void)
{
    lv_task_del(taskTimeUpdate);
    lv_page_clean(ContForWholePage);
    SetHomePageInitDate();
    printf("__Close Home Page\r\n");

}


void HomePage_Init(void)
{
    PageContCreate();
	CreatShowTimeUi();

    SetHomePageInitDate();
    taskTimeUpdate = lv_task_create((lv_task_cb_t)ReflashHomePage, 500, LV_TASK_PRIO_MID, NULL);
    lv_task_ready(taskTimeUpdate);
    Page_InitChildNode(&HomePageNode, ChildNodeList);

}


static void HomePageKey1Handl(void)
{

   // if( HomePageNode.CurChildNodeIndex < HomePageNode.ChildNodeNum)  HomePageNode.CurChildNodeIndex++;
   // else HomePageNode.CurChildNodeIndex = 0;
   Page_ChangePage(HomePageNode.ChildNode[0]);
    printf("__Key1\r\n");
}

static void HomePageKey2Handl(void)
{
    printf("__Key2\r\n");
}

static void HomePageKey3Handl(void)
{
    printf("__Key3\r\n");
}

static void HomePageKey4Handl(void)
{
    printf("__Key4\r\n");
}


PageNode_S HomePageNode = {
    .OpenPage = OpenHomePage,
    .ClosePage = CloseHomePage,
    .ReflashPage = ReflashHomePage,
    .Key1Handl = HomePageKey1Handl,
    .Key2Handl = HomePageKey2Handl,
    .Key3Handl = HomePageKey3Handl,
    .Key4Handl = HomePageKey4Handl,

};


