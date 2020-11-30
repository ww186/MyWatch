#include "Page.h"
#include "common.h"
#include <string.h>
#include "Home_Page.h"
#include "Manage_Page.h"
#include "StopWatch_Page.h"
#include "LightSetting_Page.h"
#include "TimeSet_Page.h"
#include "lvgl.h"

static PageNode_S* CurPage;

BOOL_E Page_InitChildNode(PageNode_S* iNode, PageNode_S** iNodeList)
{
    u8 i;
    iNode->ChildNodeNum = 0;

    for(i=0; i<(sizeof(iNodeList) / sizeof(PageNode_S*)); i++)
    {
        
        iNode->ChildNode[i] = iNodeList[i];
        iNode->ChildNode[i]->ParentNode = iNode;
        iNode->ChildNodeNum++;
        if(iNode->ChildNodeNum >= MAX_PAGE_CHILD_NODE_NUM) return FALSE;

    }

    return TRUE;

}

BOOL_E Page_SetParentNode(PageNode_S* iNode, PageNode_S* iParentNode)
{
   if((iNode == NULL) || (iParentNode == NULL)) return FALSE;

   if(iParentNode->ChildNodeNum < MAX_PAGE_CHILD_NODE_NUM)
   {
       iParentNode->ChildNode[iParentNode->ChildNodeNum++] = iNode;
       iNode->ParentNode = iParentNode;
       return TRUE;
   }

   return FALSE;

}

void Page_RefalshProcess(void)
{
    if(CurPage != NULL)
    {
        if(CurPage->ReflashPage != NULL) CurPage->ReflashPage();

    }

}

BOOL_E Page_SetCurrentPage(PageNode_S* iNode)
{
    if(iNode == NULL) return FALSE;

    CurPage = iNode;

    return TRUE;

}

PageNode_S* Page_GetCurrentPage(void)
{
    return CurPage;

}

void Page_Key1Handl(void)
{
     if(CurPage != NULL)
    {
        if(CurPage->Key1Handl != NULL) CurPage->Key1Handl();

    }

}

void Page_Key2Handl(void)
{
     if(CurPage != NULL)
    {
        if(CurPage->Key2Handl != NULL) CurPage->Key2Handl();

    }

}

void Page_Key3Handl(void)
{
     if(CurPage != NULL)
    {
        if(CurPage->Key3Handl != NULL) CurPage->Key3Handl();

    }

}

void Page_Key4Handl(void)
{
     if(CurPage != NULL)
    {
        if(CurPage->Key4Handl != NULL) CurPage->Key4Handl();

    }

}

void OpenPage(void)
{
    if(CurPage != NULL)
    {
        if(CurPage->OpenPage != NULL) CurPage->OpenPage();

    }
}
void ClosePage(void)
{
    if(CurPage != NULL)
    {
        if(CurPage->ClosePage != NULL) CurPage->ClosePage();

    }

}

void Page_ChangePage(PageNode_S* iNode)
{
    ClosePage();
    Page_SetCurrentPage(iNode);
    OpenPage();
}

void Page_Init(void)
{
    CurPage = &HomePageNode;
    HomePage_Init();
    ManagePage_Init();
    StopWatchPageInit();
    LightSettingPageInit();
    TimeSetPageInit();
}




lv_obj_t * lv_CreatePageCont(void)
{
    lv_obj_t * PageCont_t;
    lv_obj_t * scr = lv_scr_act();
    PageCont_t = lv_cont_create(scr, NULL);
    lv_obj_move_background(PageCont_t);
    lv_theme_t * th = lv_theme_get_current();
    lv_cont_set_style(PageCont_t, LV_CONT_STYLE_MAIN, th->style.bg);

   // lv_cont_set_layout(PageCont_t, LV_LAYOUT_OFF);
    lv_obj_set_size(PageCont_t, MAX_PAGE_WIDTH, MAX_PAGE_HIGH);
    lv_obj_align(PageCont_t, NULL, LV_ALIGN_CENTER, 0, 0);

    return PageCont_t;
}








