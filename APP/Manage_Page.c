#include "Manage_Page.h"
#include "Dev_Lcd.h"
#include "rtc.h"
#include "usart.h"	

void  ManagePage_Init(void)
{


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











