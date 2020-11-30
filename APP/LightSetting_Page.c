#include "LightSetting_Page.h"
#include "lvgl.h"
#include "Page.h"
#include "Manage_Page.h"

LV_IMG_DECLARE(ImgLight);





PageNode_S LightSettingPageNode = {
   .Name = "LightSetting",
	 .Img.ImgSrc = &ImgLight,

};



void LightSettingPageInit(void)
{
		Page_SetParentNode(&LightSettingPageNode, &MainMenuPageNode);
}



