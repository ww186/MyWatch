#include "StopWatch_Page.h"
#include "lvgl.h"
#include "Page.h"
#include "Manage_Page.h"
#include "usart.h"

LV_IMG_DECLARE(ImgStopWatch);

static lv_obj_t* StopWatchPageCont;
static lv_obj_t* TitleLab;
static lv_obj_t* Meter;
static lv_obj_t* MeterCont;

static void CreateTitleLab(lv_obj_t* Parent)
{
    TitleLab = lv_label_create(Parent, NULL);

    lv_label_set_static_text(TitleLab, "PAUSE");

    //lv_obj_set_auto_realign(TitleLab, true);

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

static lv_obj_t *CreateLmeterCont(lv_obj_t * Parent)
{
    lv_obj_t * MeterCont_t;
    MeterCont_t = lv_cont_create(Parent, NULL);

    lv_cont_set_style(MeterCont_t, LV_CONT_STYLE_MAIN, &lv_style_transp);
    lv_cont_set_fit(MeterCont_t, LV_FIT_TIGHT);
    lv_cont_set_layout(MeterCont_t, LV_LAYOUT_ROW_M);

    return MeterCont_t;
}

static lv_obj_t * CreateLmeter(lv_obj_t * Parent)
{
	lv_obj_t *Meter_t;
	static lv_style_t style_lmeter;

	Meter_t = lv_lmeter_create(Parent, NULL);
	lv_style_copy(&style_lmeter, &lv_style_pretty_color);
	 style_lmeter.body.main_color = LV_COLOR_RED;
    style_lmeter.body.grad_color = LV_COLOR_RED;
    style_lmeter.body.padding.left = 5;
    style_lmeter.line.width = 1;
    style_lmeter.line.color = LV_COLOR_SILVER;
	lv_line_set_style(Meter_t, LV_LINE_STYLE_MAIN, &style_lmeter);

    lv_obj_set_size(Meter_t, 70, 70);
    lv_lmeter_set_range(Meter_t, 0, 59);
    lv_lmeter_set_value(Meter_t, 0);
    lv_lmeter_set_scale(Meter_t, 360, 31);
   // lv_lmeter_set_angle_offset(Meter_t, 180);


	return Meter_t;
}

static void OpenStopWatchPage(void)
{
    CreateTitleLab(StopWatchPageCont);
	CreateLine(StopWatchPageCont);
    MeterCont = CreateLmeterCont(StopWatchPageCont);
	CreateLmeter(MeterCont);

    lv_obj_move_foreground(StopWatchPageCont);
    printf("__Open StopWatch Page\r\n");
}

static void CloseStopWatchPage(void)
{
     lv_obj_move_background(StopWatchPageCont);
     lv_obj_clean(StopWatchPageCont);
    printf("__Close StopWatch Page\r\n");

}


static void StopWatchPageKey1Handl(void)
{
   
   printf("__StopWatchKey1\r\n");
}

static void StopWatchPageKey2Handl(void)
{
    Page_ChangePage(StopWatchPageNode.ParentNode);
    printf("__StopWatchKey2\r\n");
}

static void StopWatchPageKey3Handl(void)
{
    
    printf("__StopWatchKey3\r\n");
}

static void StopWatchPageKey4Handl(void)
{
    
    printf("__StopWatchKey4\r\n");
}



PageNode_S StopWatchPageNode = {
   .Name = "StopWatch",
   .Img.ImgSrc = &ImgStopWatch,
   .OpenPage = OpenStopWatchPage,
	.ClosePage = CloseStopWatchPage,
	.Key1Handl = StopWatchPageKey1Handl,
	.Key2Handl = StopWatchPageKey2Handl,
	.Key3Handl = StopWatchPageKey3Handl,
	.Key4Handl = StopWatchPageKey4Handl,

};

static lv_obj_t * CreateStopWatchPageCont(void)
{
	static lv_style_t StopWatchPageContStyle;
	lv_obj_t * StopWatchPageCont_t;
    StopWatchPageCont_t = lv_CreatePageCont();
	lv_style_copy(&StopWatchPageContStyle, lv_cont_get_style(StopWatchPageCont_t, LV_CONT_STYLE_MAIN));
    StopWatchPageContStyle.body.padding.inner = 7;
    lv_cont_set_style(StopWatchPageCont_t, LV_CONT_STYLE_MAIN, &StopWatchPageContStyle);
    lv_cont_set_layout(StopWatchPageCont_t, LV_LAYOUT_CENTER);
    return StopWatchPageCont_t;
}


void StopWatchPageInit(void)
{
	StopWatchPageCont = CreateStopWatchPageCont();
	//CreateTitleLab(StopWatchPageCont);
	//CreateLine(StopWatchPageCont);


	Page_SetParentNode(&StopWatchPageNode, &MainMenuPageNode);
}



