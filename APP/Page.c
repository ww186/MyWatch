#include "Page.h"
#include "common.h"
#include "Home_Page.h"
#include <string.h>

static PageNode_S* CurPage;

BOOL_E Page_InitChildNode(PageNode_S* iNode, PageNode_S** iNodeList)
{
    u8 i;
    iNode->ChildNodeNum = 0;

    for(i=0; i<(sizeof(iNodeList) / sizeof(PageNode_S*)); i++)
    {
        
        iNode->ChildNode[i] = iNodeList[i];
        iNode->ChildNodeNum++;
        if(iNode->ChildNodeNum >= MAX_PAGE_CHILD_NODE_NUM) return FALSE;

    }

    return TRUE;

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

void Page_Init(void)
{
    CurPage = &HomePageNode;
}








