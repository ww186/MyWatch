#include "lvgl.h"
#include "common.h"


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


