#include "Home_Page.h"
#include "Dev_Lcd.h"
#include "rtc.h"
#include "usart.h"
#include "lvgl.h"	

PageNode_S* ChildNodeList[] = {
    &ManagePageNode,
};

static Date_S Date;

static lv_obj_t* Hour_1;
static lv_obj_t* Hour_2;
static lv_obj_t* Min_1;
static lv_obj_t* Min_2;
static lv_obj_t* Sec_1;
static lv_obj_t* Sec_2;
static lv_obj_t* Dot;

static lv_obj_t* cont;


void HomePage_Init(void)
{
    lv_obj_t * scr = lv_scr_act();

	cont = lv_cont_create(scr, NULL);
	
	lv_cont_set_layout(cont, LV_LAYOUT_ROW_M);
	lv_cont_set_fit(cont, LV_FIT_NONE);
	lv_obj_set_size(cont, 300, 200);
	lv_obj_align(cont, NULL, LV_ALIGN_CENTER, 0, 0);
    Hour_1 = lv_label_create(cont, NULL);
	Hour_2 = lv_label_create(cont, NULL);
    Dot = lv_label_create(cont, NULL);
    Min_1 = lv_label_create(cont, NULL);
	Min_2 = lv_label_create(cont, NULL);
    Sec_1 = lv_label_create(cont, NULL);
	Sec_2 = lv_label_create(cont, NULL);
	
	lv_label_set_text(Dot, ":");

    Date.Hour = 0xff;
    Date.Min = 0xff;
    Date.Sec = 0xff;
    Page_InitChildNode(&HomePageNode, ChildNodeList);

}

static void DisplayNum(lv_obj_t* Lab, u8 Num)
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

static void DisplayDate(Date_S* const iDate)
{
    if(Date.Hour != iDate->Hour)
	{
		
		if(Date.Hour/10 != iDate->Hour/10)
			DisplayNum(Hour_1, iDate->Hour/10);
		if(Date.Hour%10 != iDate->Hour%10)
			DisplayNum(Hour_2, iDate->Hour%10);
		
		Date.Hour = iDate->Hour;
	}
	
	if(Date.Min != iDate->Min)
	{
		
		if(Date.Min/10 != iDate->Min/10)
			DisplayNum(Min_1, iDate->Min/10);
		if(Date.Min%10 != iDate->Min%10)
			DisplayNum(Min_2, iDate->Min%10);
		
		Date.Min = iDate->Min;
	}
	
	if(Date.Sec != iDate->Sec)
	{
		
		if(Date.Sec/10 != iDate->Sec/10)
			DisplayNum(Sec_1, iDate->Sec/10);
		if(Date.Sec%10 != iDate->Sec%10)
			DisplayNum(Sec_2, iDate->Sec%10);
		
		Date.Sec = iDate->Sec;
	}
	
}

static void OpenHomePage(void)
{
    Rtc_InitDate();
    printf("__Open Home Page\r\n");
}

static void CloseHomePage(void)
{
    App_Clear();
    printf("__Close Home Page\r\n");

}



static void ReflashHomePage(void)
{
    Date_S Date_t;

    Rtc_GetDate(&Date_t);
    DisplayDate(&Date_t);
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


