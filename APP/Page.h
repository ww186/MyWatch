#ifndef __PAGE_H
#define __PAGE_H	
#include "Config.h"

typedef struct PageNode{
    u8 ChildNodeNum;
	struct PageNode* ChildNode[MAX_PAGE_CHILD_NODE_NUM];
    void (*OpenPage)(void);
    void (*ClosePage)(void);
    void (*ReflashPage)(void);
    void (*Key1Handl)(void);
    void (*Key2Handl)(void);
    void (*Key3Handl)(void);
    void (*Key4Handl)(void);

}PageNode_S;

void Page_Init(void);
BOOL_E Page_InitChildNode(PageNode_S* iNode, PageNode_S** iNodeList);
void Page_RefalshProcess(void);
BOOL_E Page_SetCurrentPage(PageNode_S* iNode);
PageNode_S* Page_GetCurrentPage(void);
void Page_Key1Handl(void);
void Page_Key2Handl(void);
void Page_Key3Handl(void);
void Page_Key4Handl(void);

#endif



