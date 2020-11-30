#include "TimeSet_Page.h"
#include "lvgl.h"
#include "Page.h"
#include "Manage_Page.h"

LV_IMG_DECLARE(ImgTimeCfg);





PageNode_S TimeSetPageNode = {
   .Name = "TimeSet",
	 .Img.ImgSrc = &ImgTimeCfg,

};



void TimeSetPageInit(void)
{
		Page_SetParentNode(&TimeSetPageNode, &MainMenuPageNode);
}


