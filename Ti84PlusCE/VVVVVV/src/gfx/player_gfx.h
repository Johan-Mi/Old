// convpng v7.2
// this file contains all the graphics sources for easy inclusion in a project
#ifndef __player_gfx__
#define __player_gfx__
#include <stdint.h>

#define player_gfx_transparent_color_index 0

#define player_width 10
#define player_height 21
#define player_size 212
extern uint8_t player_data[212];
#define player ((gfx_sprite_t*)player_data)
#define sizeof_player_gfx_pal 4
extern uint16_t player_gfx_pal[2];

#endif
