#include "lvgl.h"
#include "common.h"
#include <stdlib.h>
#include <string.h>

LinkList* CreateLinkList(void)
{
	LinkList* LinkList_t = (LinkList*)malloc(sizeof(LinkList));

	if(LinkList_t == NULL) return NULL;

	LinkList_t->NodeNum = 0;
	LinkList_t->HeadNode = NULL;
	LinkList_t->TailNode = NULL;

	return LinkList_t;
}

LinkListNode_S* CreateLinkListNode(void* iData, int DataSize)
{
	if(DataSize == 0) return NULL;
	LinkListNode_S* Node_t = (LinkListNode_S*)malloc(sizeof(LinkListNode_S));
	if(Node_t == NULL) return NULL;
	void* Data_t = malloc(DataSize);
	if(Data_t == NULL) return NULL;

	memcpy(Data_t, iData, DataSize);
	Node_t->Data = Data_t;
	Node_t->NextNode = NULL;

	return Node_t;
}

BOOL_E LinkList_AddNodeToTail(LinkList* iLinkList, LinkListNode_S* iNode)
{
    if((iNode == NULL) || (iLinkList == NULL)) return FALSE;

    if(iLinkList->NodeNum == 0) 
    {
        iLinkList->HeadNode = iNode;
        iLinkList->TailNode = iNode;
    }
    else
	{
        iLinkList->TailNode->NextNode = iNode;
	    iLinkList->TailNode = iNode;
    }
	
    iLinkList->NodeNum++;

    return TRUE;
}



void lv_AddAnimObj(lv_obj_t* obj, lv_coord_t Start_y, lv_coord_t End_y, lv_anim_exec_xcb_t exec_cb, uint16_t time)
{
    lv_coord_t AnimStart_y, AnimEnd_y;
    lv_anim_t a;
    AnimStart_y = Start_y;
    AnimEnd_y = End_y;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, obj, (lv_anim_exec_xcb_t)exec_cb);
    lv_anim_set_values(&a, AnimStart_y, AnimEnd_y);
    lv_anim_set_path_cb(&a, lv_anim_path_linear);
    lv_anim_set_time(&a, time, 0);

    lv_anim_create(&a);
}

