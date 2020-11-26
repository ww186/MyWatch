#include "StopWatch_Page.h"
#include "lvgl.h"
#include "Page.h"

LV_IMG_DECLARE(ImgStopWatch);


void StopWatchPageInit(void)
{
		Page_SetParentNode(&StopWatchPageNode, &ManagePageNode);
}





PageNode_S StopWatchPageNode = {
   .Name = "StopWatch",
	 .Img.ImgSrc = &ImgStopWatch,

};







