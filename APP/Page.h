#ifndef __PAGE_H
#define __PAGE_H	
#include "Config.h"
#include "lvgl.h"
//#include "Home_Page.h"
//#include "Manage_Page.h"

typedef struct{
    lv_obj_t* ImgObj;
    const lv_img_dsc_t* ImgSrc;
}NodeImg;

typedef struct PageNode{
    u8 ChildNodeNum;
    char Name[MAX_PAGE_NAME_LEN];
    NodeImg Img;
    //u8 CurChildNodeIndex;
    struct PageNode* ParentNode;
	struct PageNode* ChildNode[MAX_PAGE_CHILD_NODE_NUM];
    void (*OpenPage)(void);
    void (*ClosePage)(void);
    void (*ReflashPage)(void);
    void (*Key1Handl)(void);
    void (*Key2Handl)(void);
    void (*Key3Handl)(void);
    void (*Key4Handl)(void);

}PageNode_S;

//extern PageNode_S HomePageNode;
//extern PageNode_S ManagePageNode;
//extern PageNode_S StopWatchPageNode;




void Page_Init(void);
BOOL_E Page_InitChildNode(PageNode_S* iNode, PageNode_S** iNodeList);
BOOL_E Page_SetParentNode(PageNode_S* iNode, PageNode_S* iParentNode);
void Page_RefalshProcess(void);
BOOL_E Page_SetCurrentPage(PageNode_S* iNode);
PageNode_S* Page_GetCurrentPage(void);
void OpenPage(void);
void ClosePage(void);
void Page_Key1Handl(void);
void Page_Key2Handl(void);
void Page_Key3Handl(void);
void Page_Key4Handl(void);
void Page_ChangePage(PageNode_S* iNode);

lv_obj_t * lv_CreatePageCont(void);

#endif



