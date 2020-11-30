#include "Manage_Page.h"
#include "usart.h"	
#include "lvgl.h"
#include "Page.h"
#include "Home_Page.h"


static u8 CurChildNodeIndex;

static lv_obj_t* MainMenuPageCont;
static lv_obj_t* TitleCont;
static lv_obj_t* TitleLab;
static lv_obj_t* ImgCont;
static lv_obj_t* SplitLine;

static lv_obj_t * CreateMainMenuPageCont(void)
{
    lv_obj_t * MainMenuPageCont_t;
    MainMenuPageCont_t = lv_CreatePageCont();

    lv_cont_set_layout(MainMenuPageCont_t, LV_LAYOUT_OFF);
    return MainMenuPageCont_t;
}

static lv_obj_t * CreateTitleCont(lv_obj_t* Parent)
{

    lv_obj_t * TitleCont_t;
    TitleCont_t = lv_cont_create(Parent, NULL);

    lv_cont_set_style(TitleCont_t, LV_CONT_STYLE_MAIN, &lv_style_transp);
    lv_cont_set_fit(TitleCont_t, LV_FIT_TIGHT);
    lv_obj_align(TitleCont_t, Parent, LV_ALIGN_IN_TOP_MID, 0, 7);

    return TitleCont_t;
}


static void CreateTitleLab(lv_obj_t* Parent)
{
    TitleLab = lv_label_create(Parent, NULL);

    lv_label_set_static_text(TitleLab, MainMenuPageNode.ChildNode[0]->Name);
   // lv_obj_align(TitleLab, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
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
    //lv_obj_align(ImgCont_t, Parent, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

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

static void MoveImgMenuAnim(lv_obj_t * MenuCont, lv_coord_t Start, lv_coord_t End)
{
    lv_AddAnimObj(MenuCont, Start, End, (lv_anim_exec_xcb_t)lv_obj_set_x, LV_ANIM_TIME_DEFAULT);
}


static void MoveToNextImg(u8 Index)
{
    
    lv_coord_t AnimStart_x, AnimEnd_x;
    AnimStart_x = lv_obj_get_x(ImgCont);
    AnimEnd_x = 75 - Index*100;

    MoveImgMenuAnim(ImgCont, AnimStart_x, AnimEnd_x);
}


static lv_obj_t * CreateLine(lv_obj_t * Parent)
{
    static const lv_point_t  point[]= {{0, 0}, {MAX_PAGE_WIDTH, 0}};

    lv_obj_t * SplitLine_t;
    SplitLine_t = lv_line_create(Parent, NULL);
    lv_line_set_points(SplitLine_t, point, 2);

    static lv_style_t style_line;
    style_line = *lv_line_get_style(SplitLine_t, LV_LINE_STYLE_MAIN);
    style_line.line.color = LV_COLOR_RED;
    style_line.line.width = 2;
    style_line.line.rounded = 1;
    lv_line_set_style(SplitLine_t, LV_LINE_STYLE_MAIN, &style_line);

    return SplitLine_t;
}

static void CreatePageObj(void)
{
    TitleCont = CreateTitleCont(MainMenuPageCont);
    CreateTitleLab(TitleCont);
    SplitLine = CreateLine(MainMenuPageCont);
    lv_obj_align(SplitLine, TitleCont, LV_ALIGN_OUT_BOTTOM_MID, 0, 7);

    ImgCont = CreateImgCont(MainMenuPageCont);
    lv_obj_align(ImgCont, SplitLine, LV_ALIGN_OUT_BOTTOM_RIGHT, 50, 20);
    CreateImgObj(ImgCont, &MainMenuPageNode);
}

static void OpenMainMenuPage(void)
{
    CreatePageObj();
    lv_obj_move_foreground(MainMenuPageCont);
    CurChildNodeIndex = 0;
    MoveToNextImg(CurChildNodeIndex);
    printf("__Open MainMenu Page\r\n");
}

static void CloseMainMenuPage(void)
{
    //App_Clear();
     lv_obj_move_background(MainMenuPageCont);
     CurChildNodeIndex = 0;
     lv_obj_clean(MainMenuPageCont);
    printf("__Close MainMenu Page\r\n");

}


static void MainMenuPageKey1Handl(void)
{
   Page_ChangePage(MainMenuPageNode.ChildNode[CurChildNodeIndex]);
   printf("__MainMenuKey1\r\n");
}

static void MainMenuPageKey2Handl(void)
{
    Page_ChangePage(MainMenuPageNode.ParentNode);
    printf("__MainMenuKey2\r\n");
}

static void MainMenuPageKey3Handl(void)
{
    if(CurChildNodeIndex < (MainMenuPageNode.ChildNodeNum-1))
        CurChildNodeIndex++;
    else
        CurChildNodeIndex = 0; 

    MoveToNextImg(CurChildNodeIndex);
    lv_label_set_static_text(TitleLab, MainMenuPageNode.ChildNode[CurChildNodeIndex]->Name);
    
    printf("__MainMenuKey3\r\n");
}

static void MainMenuPageKey4Handl(void)
{
    if(CurChildNodeIndex > 0)
        CurChildNodeIndex--;
    else
        CurChildNodeIndex = MainMenuPageNode.ChildNodeNum - 1;

    MoveToNextImg( CurChildNodeIndex);
    lv_label_set_static_text(TitleLab, MainMenuPageNode.ChildNode[CurChildNodeIndex]->Name);
    
    printf("__MainMenuKey4\r\n");
}


PageNode_S MainMenuPageNode = {
    .OpenPage = OpenMainMenuPage,
    .ClosePage = CloseMainMenuPage,
    .Key1Handl = MainMenuPageKey1Handl,
    .Key2Handl = MainMenuPageKey2Handl,
    .Key3Handl = MainMenuPageKey3Handl,
    .Key4Handl = MainMenuPageKey4Handl,

};


void  ManagePage_Init(void)
{
   
   MainMenuPageCont = CreateMainMenuPageCont();
   //CreatePageObj();
   CurChildNodeIndex = 0;

   Page_SetParentNode(&MainMenuPageNode, &HomePageNode);
}








