#include "Manage_Page.h"
#include "Dev_Lcd.h"
#include "rtc.h"
#include "usart.h"	
#include "lvgl.h"	

static lv_obj_t* ManagePage;

void  ManagePage_Init(void)
{
    lv_obj_t * scr = lv_scr_act();
    ManagePage = lv_page_create(scr, NULL);
    lv_obj_move_background(ManagePage);
    
    lv_page_set_scrl_layout(ManagePage, LV_LAYOUT_COL_M);

    lv_page_set_sb_mode(ManagePage, LV_SB_MODE_OFF);
	lv_obj_set_size(ManagePage, 200, 120);
	lv_obj_align(ManagePage, NULL, LV_ALIGN_CENTER, 0, 0);

}

static void OpenManagePage(void)
{
    printf("__Open Manage Page\r\n");
}

static void CloseManagePage(void)
{
    App_Clear();
    printf("__Close Manage Page\r\n");

}

static void ReflashManagePage(void)
{
  
}

static void ManagePageKey1Handl(void)
{
   Page_ChangePage(ManagePageNode.ParentNode);
   printf("__ManageKey1\r\n");
}

static void ManagePageKey2Handl(void)
{
    printf("__ManageKey2\r\n");
}

static void ManagePageKey3Handl(void)
{
    printf("__ManageKey3\r\n");
}

static void ManagePageKey4Handl(void)
{
    printf("__ManageKey4\r\n");
}


PageNode_S ManagePageNode = {
    .OpenPage = OpenManagePage,
    .ClosePage = CloseManagePage,
    .ReflashPage = ReflashManagePage,
    .Key1Handl = ManagePageKey1Handl,
    .Key2Handl = ManagePageKey2Handl,
    .Key3Handl = ManagePageKey3Handl,
    .Key4Handl = ManagePageKey4Handl,

};











