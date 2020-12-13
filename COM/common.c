#include "lvgl.h"
#include "common.h"
#include "ff.h"
#include <stdlib.h>
#include <string.h>

static FATFS fs;
static BYTE work[FF_MAX_SS];


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


static  bool IsFatFsReady(lv_fs_drv_t * drv)
{
    FRESULT res;
    lv_fs_drv_t *p = drv;

//    res = f_mount(&fs,"1:",1);
//    if(res == FR_OK) return true;

//    res = f_mkfs("1:", 0, work, sizeof(work));
//    if(res == FR_OK) 
//    {
//         res = f_mount(&fs,"1:",1);
//         if(res == FR_OK) return true;
//    }

//    return false;
	return true;
}

static lv_fs_res_t f_Open(struct _lv_fs_drv_t * drv, void * file_p, const char * path, lv_fs_mode_t mode)
{
    FRESULT res;
    lv_fs_drv_t *p = drv;

    res = f_open(file_p, path, mode);
    if(res == FR_OK) return LV_FS_RES_OK;

    return LV_FS_RES_UNKNOWN;
}

static lv_fs_res_t f_Close(struct _lv_fs_drv_t * drv, void * file_p)
{
    FRESULT res;
    lv_fs_drv_t *p = drv;

    res = f_close(file_p);

    if(res == FR_OK) return LV_FS_RES_OK;

    return LV_FS_RES_UNKNOWN;
}

static lv_fs_res_t f_Read(struct _lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br)
{
    FRESULT res;
    lv_fs_drv_t *p = drv;

    res = f_read(file_p, buf, btr, br);

    if(res == FR_OK) return LV_FS_RES_OK;

    return LV_FS_RES_UNKNOWN;
}

static lv_fs_res_t f_Write(struct _lv_fs_drv_t * drv, void * file_p, const void * buf, uint32_t btw, uint32_t * bw)
{
    FRESULT res;
    lv_fs_drv_t *p = drv;

    res = f_write(file_p, buf, btw, bw);

    if(res == FR_OK) return LV_FS_RES_OK;

    return LV_FS_RES_UNKNOWN;
}

lv_fs_drv_t FsDrv = {
	//.ready_cb = IsFatFsReady,
    .letter = '1',
    .file_size = sizeof(FIL),
    .open_cb = f_Open,
    .close_cb = f_Close,
    .read_cb = f_Read,
    .write_cb = f_Write,
};

BOOL_E FatFsInit(void)
{
    FRESULT res;

    lv_fs_drv_register(&FsDrv);

    res = f_mount(&fs,"1:",1);
    if(res == FR_OK) return TRUE;

    res = f_mkfs("1:", 0, work, sizeof(work));
    if(res == FR_OK) 
    {
         res = f_mount(&fs,"1:",1);
         if(res == FR_OK) return TRUE;
    }

    return FALSE;
}





