#include "Manage_Page.h"
#include "usart.h"	
#include "lvgl.h"

//LV_IMG_DECLARE(ImgButterfly);
//LV_IMG_DECLARE(ImgInfo);
LV_IMG_DECLARE(ImgLight);
LV_IMG_DECLARE(ImgSettings);
//LV_IMG_DECLARE(ImgMountain);
LV_IMG_DECLARE(ImgTimeCfg);

static u8 CurChildNodeIndex;

static lv_obj_t* ManagePage;
static lv_obj_t* TitleCont;
static lv_obj_t* TitleLab;
static lv_obj_t* ImgCont;




static lv_obj_t * CreateManagePage(void)
{
    lv_obj_t * ManagePage_t;
    lv_obj_t * scr = lv_scr_act();
    ManagePage_t = lv_cont_create(scr, NULL);
    lv_obj_move_background(ManagePage_t);
    lv_theme_t * th = lv_theme_get_current();
    lv_cont_set_style(ManagePage_t, LV_CONT_STYLE_MAIN, th->style.bg);

    lv_cont_set_layout(ManagePage_t, LV_LAYOUT_OFF);
    lv_obj_set_size(ManagePage_t, 200, 120);
    lv_obj_align(ManagePage_t, NULL, LV_ALIGN_CENTER, 0, 0);

    return ManagePage_t;
}

static lv_obj_t * CreateTitleCont(lv_obj_t* Parent)
{
    lv_obj_t * TitleCont_t;
    TitleCont_t = lv_cont_create(Parent, NULL);

    lv_cont_set_style(TitleCont_t, LV_CONT_STYLE_MAIN, &lv_style_transp);
    lv_cont_set_fit(TitleCont_t, LV_FIT_TIGHT);
    lv_obj_align(TitleCont_t, Parent, LV_ALIGN_IN_TOP_MID, 0, 0);

    return TitleCont_t;
}

static void CreateTitleLab(lv_obj_t* Parent)
{
    TitleLab = lv_label_create(Parent, NULL);

    lv_label_set_static_text(TitleLab, ManagePageNode.ChildNode[0]->Name);
    lv_obj_align(TitleLab, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_set_auto_realign(TitleLab, true);

}

static lv_obj_t * CreateImgCont(lv_obj_t* Parent)
{
    lv_obj_t * ImgCont_t;
   static lv_style_t ImgContStyle;
    ImgCont_t = lv_cont_create(Parent, NULL);
    ImgContStyle = lv_style_transp;
    ImgContStyle.body.padding.inner = 50;

    lv_cont_set_style(ImgCont_t, LV_CONT_STYLE_MAIN, &ImgContStyle);
    lv_cont_set_layout(ImgCont_t, LV_LAYOUT_ROW_M);
    lv_cont_set_fit(ImgCont_t, LV_FIT_TIGHT);
    lv_obj_align(ImgCont_t, Parent, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

    return ImgCont_t;

}

static void CreateImgObj(lv_obj_t* Parent, PageNode_S* iNode)
{
    u8 i;

    for(i=0; i<iNode->ChildNodeNum; i++)
    {
        if(iNode->ChildNode[i]->Img.ImgSrc != NULL)
        {
            iNode->ChildNode[i]->Img.ImgObj = lv_img_create(Parent, NULL);
            lv_img_set_src(iNode->ChildNode[i]->Img.ImgObj , iNode->ChildNode[i]->Img.ImgSrc);
        }
       

    }
}

static void MoveToNextImg(u8 Index)
{
    
    lv_coord_t AnimStart_x, AnimEnd_x;
    lv_anim_t a;
    AnimStart_x = lv_obj_get_x(ImgCont);
   
    AnimEnd_x = 75 - Index*100;
    //AnimEnd_y = End_y;

    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, ImgCont, (lv_anim_exec_xcb_t)lv_obj_set_x);
    lv_anim_set_values(&a, AnimStart_x, AnimEnd_x);
    lv_anim_set_path_cb(&a, lv_anim_path_linear);
    lv_anim_set_time(&a, LV_ANIM_TIME_DEFAULT, 0);

    lv_anim_create(&a);
}

static void MoveToForwordImg(u8 Index)
{
    
    lv_coord_t AnimStart_x, AnimEnd_x;
    lv_anim_t a;
    AnimStart_x = lv_obj_get_x(ImgCont);
   
    AnimEnd_x = 75 + Index*100;
    //AnimEnd_y = End_y;

    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, ImgCont, (lv_anim_exec_xcb_t)lv_obj_set_x);
    lv_anim_set_values(&a, AnimStart_x, AnimEnd_x);
    lv_anim_set_path_cb(&a, lv_anim_path_linear);
    lv_anim_set_time(&a, LV_ANIM_TIME_DEFAULT, 0);

    lv_anim_create(&a);
}


void  ManagePage_Init(void)
{
   
   ManagePage = CreateManagePage();
   TitleCont = CreateTitleCont(ManagePage);
   CreateTitleLab(TitleCont);
   ImgCont = CreateImgCont(ManagePage);
   CreateImgObj(ImgCont, &ManagePageNode);
   CurChildNodeIndex = 0;

   Page_SetParentNode(&ManagePageNode, &HomePageNode);
}

static void OpenManagePage(void)
{
    TitleCont = CreateTitleCont(ManagePage);
    CreateTitleLab(TitleCont);
    ImgCont = CreateImgCont(ManagePage);
    CreateImgObj(ImgCont, &ManagePageNode);
    lv_obj_move_foreground(ManagePage);
    CurChildNodeIndex = 0;
    MoveToNextImg(CurChildNodeIndex);
    printf("__Open Manage Page\r\n");
}

static void CloseManagePage(void)
{
    //App_Clear();
     lv_obj_move_background(ManagePage);
     CurChildNodeIndex = 0;
     lv_obj_clean(ManagePage);
    printf("__Close Manage Page\r\n");

}

static void ReflashManagePage(void)
{
  
}

static void ManagePageKey1Handl(void)
{
   
   printf("__ManageKey1\r\n");
}

static void ManagePageKey2Handl(void)
{
    Page_ChangePage(ManagePageNode.ParentNode);
    printf("__ManageKey2\r\n");
}

static void ManagePageKey3Handl(void)
{
    if(CurChildNodeIndex < (ManagePageNode.ChildNodeNum-1))
        CurChildNodeIndex++;
    else
        CurChildNodeIndex = 0; 

    MoveToNextImg(CurChildNodeIndex);
    lv_label_set_static_text(TitleLab, ManagePageNode.ChildNode[CurChildNodeIndex]->Name);
    
    printf("__ManageKey3\r\n");
}

static void ManagePageKey4Handl(void)
{
    if(CurChildNodeIndex > 0)
        CurChildNodeIndex--;
    else
        CurChildNodeIndex = ManagePageNode.ChildNodeNum - 1;

    MoveToForwordImg( CurChildNodeIndex);
    lv_label_set_static_text(TitleLab, ManagePageNode.ChildNode[CurChildNodeIndex]->Name);
    
    printf("__ManageKey4\r\n");
}


PageNode_S ManagePageNode = {
    .OpenPage = OpenManagePage,
    .ClosePage = CloseManagePage,
    .Key1Handl = ManagePageKey1Handl,
    .Key2Handl = ManagePageKey2Handl,
    .Key3Handl = ManagePageKey3Handl,
    .Key4Handl = ManagePageKey4Handl,

};











