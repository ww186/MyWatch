#include "Manage_Page.h"
#include "Dev_Lcd.h"
#include "rtc.h"
#include "usart.h"	
#include "lvgl.h"

//LV_IMG_DECLARE(ImgButterfly);
//LV_IMG_DECLARE(ImgInfo);
LV_IMG_DECLARE(ImgLight);
LV_IMG_DECLARE(ImgSettings);
//LV_IMG_DECLARE(ImgMountain);
LV_IMG_DECLARE(ImgTimeCfg);

static lv_obj_t* ManagePage;
static lv_obj_t* TitleCont;
static lv_obj_t* ImgCont;
#define SELEC_NUM sizeof(SelecList)/sizeof(ManageSelec_S)

typedef struct{
    const lv_img_dsc_t* src_img;
    u8 Title[16];
    lv_obj_t * img; 
}ManageSelec_S;

ManageSelec_S SelecList[] = {
    {&ImgSettings, "Setting"},
    {&ImgLight, "Light"},
    {&ImgTimeCfg, "StopWatch"},
};



static void CreateManagePage(void)
{
    lv_obj_t * scr = lv_scr_act();
    ManagePage = lv_cont_create(scr, NULL);
    lv_obj_move_background(ManagePage);
    lv_theme_t * th = lv_theme_get_current();
    lv_cont_set_style(ManagePage, LV_CONT_STYLE_MAIN, th->style.bg);

    lv_cont_set_layout(ManagePage, LV_LAYOUT_OFF);
    lv_obj_set_size(ManagePage, 200, 120);
    lv_obj_align(ManagePage, NULL, LV_ALIGN_CENTER, 0, 0);
}

static void CreateTitleCont(lv_obj_t* Parent)
{
    TitleCont = lv_cont_create(Parent, NULL);

    lv_cont_set_style(TitleCont, LV_CONT_STYLE_MAIN, &lv_style_transp);
    lv_cont_set_fit(TitleCont, LV_FIT_TIGHT);
    lv_obj_align(TitleCont, Parent, LV_ALIGN_IN_TOP_MID, 0, 0);

}

static void CreateImgCont(lv_obj_t* Parent)
{
   static lv_style_t ImgContStyle;
    ImgCont = lv_cont_create(Parent, NULL);
    ImgContStyle = lv_style_transp;
    ImgContStyle.body.padding.inner = 50;

    lv_cont_set_style(ImgCont, LV_CONT_STYLE_MAIN, &ImgContStyle);
    lv_cont_set_layout(ImgCont, LV_LAYOUT_ROW_M);
    lv_cont_set_fit(ImgCont, LV_FIT_TIGHT);
    lv_obj_align(ImgCont, Parent, LV_ALIGN_CENTER, 0, 0);

}

static void CreateImgObj(lv_obj_t* Parent, ManageSelec_S* List)
{
    u8 i;

    for(i=0; i<SELEC_NUM; i++)
    {
        List->img = lv_img_create(Parent, NULL);
        lv_img_set_src(List->img, List->src_img);
        //lv_obj_align(List->img, NULL, LV_ALIGN_IN_TOP_MID, i*95, 0);

    }
}

static void lv_anim_exec_cb_num(lv_obj_t* Lab, lv_anim_value_t coord)
{
    printf("__lv_anim_exec_cb_num Coord %d \r\n", coord);
    lv_obj_set_x(Lab, coord);
}

static void MoveToNextImg(void)
{
    
    lv_coord_t AnimStart_x, AnimEnd_x;
    lv_anim_t a;
    AnimStart_x = lv_obj_get_x(ImgCont);
    if(ManagePageNode.ChildNodeNum == ManagePageNode.CurChildNodeIndex)
        AnimEnd_x = 0;
    else
        AnimEnd_x = AnimStart_x + 95;
    //AnimEnd_y = End_y;

    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, ImgCont, (lv_anim_exec_xcb_t)lv_anim_exec_cb_num);
    //lv_anim_set_ready_cb(&a, lv_anim_Ready_cb_num );
    lv_anim_set_values(&a, AnimStart_x, AnimEnd_x);
    lv_anim_set_path_cb(&a, lv_anim_path_linear);
    lv_anim_set_time(&a, LV_ANIM_TIME_DEFAULT, 0);

    lv_anim_create(&a);
}


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











