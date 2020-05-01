// convpng v7.2
// this file contains all the graphics sources for easy inclusion in a project
#ifndef __image_gfx__
#define __image_gfx__
#include <stdint.h>

#define image_gfx_transparent_color_index 0

#define image_width 48
#define image_height 28
#define image_size 1346
extern uint8_t image_data[1346];
#define image ((gfx_sprite_t*)image_data)
#define sizeof_image_gfx_pal 4
extern uint16_t image_gfx_pal[2];

#endif
