#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <graphx.h>
#include <keypadc.h>

#include "gfx/player_gfx.h"
#include "gfx/tiles_gfx.h"

extern uint8_t mapData[];

void main(void) {
	sk_key_t key;
	int player_x = 0;
	int player_y = 0;
	bool player_x_subpixel = 0;
	bool flip_pressed = false;
	bool player_xdir = true;

	int gravity = 1;

	gfx_sprite_t* active_player_sprite = player;
	gfx_UninitedSprite(player_buffer, player_width, player_height);

	gfx_tilemap_t tilemap;
	tilemap.map = mapData;
	tilemap.tiles = tiles_tiles;
	tilemap.type_width = gfx_tile_8_pixel;
	tilemap.type_height = gfx_tile_8_pixel;
	tilemap.tile_width = 8;
	tilemap.tile_height = 8;
	tilemap.draw_width = 20;
	tilemap.width = 20;
	tilemap.draw_height = 14;
	tilemap.height = 14;
	tilemap.x_loc = 0;
	tilemap.y_loc = 0;
	
	gfx_Begin();
	gfx_SetDrawBuffer();
	gfx_SetPalette(tiles_gfx_pal, sizeof_tiles_gfx_pal, 0);

	while(!kb_IsDown(kb_KeyClear)) {
		gfx_ZeroScreen();
		gfx_Tilemap_NoClip(&tilemap, 0, 0);
		gfx_Sprite(active_player_sprite, player_x, player_y);
		gfx_SwapDraw();

		if(kb_IsDown(kb_KeyLeft)) {
			player_x -= 1 + (player_x_subpixel ^= 1);
			if(player_x < 0) {
				player_x = 0;
				player_x_subpixel = 0;
			}
			if(player_xdir) {
				player_xdir = false;
				active_player_sprite = gfx_FlipSpriteY(active_player_sprite, (active_player_sprite == player ? player_buffer : player));
			}
		}
		if(kb_IsDown(kb_KeyRight)) {
			player_x += 1 + (player_x_subpixel ^= 1);
			if(player_x > LCD_WIDTH - player_data[0]) {
				player_x = LCD_WIDTH - player_data[0];
				player_x_subpixel = 0;
			}
			if(!player_xdir) {
				active_player_sprite = gfx_FlipSpriteY(active_player_sprite, (active_player_sprite == player ? player_buffer : player));
				player_xdir = true;
			}
		}
		if(kb_IsDown(kb_Key2nd)) {
			if(!flip_pressed) {
				gravity = -gravity;
				active_player_sprite = gfx_FlipSpriteX(active_player_sprite, (active_player_sprite == player_buffer ? player : player_buffer));
				flip_pressed = true;
			}
		} else
			flip_pressed = false;
	}

	gfx_End();
}
