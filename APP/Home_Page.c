#include "Home_Page.h"
#include "Dev_Lcd.h"
#include "rtc.h"
#include "usart.h"	

PageNode_S* ChildNodeList[] = {
    &ManagePageNode,
};

void HomePage_Init(void)
{
    Page_InitChildNode(&HomePageNode, ChildNodeList);

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
    App_DisplayTime(&Date_t);
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

