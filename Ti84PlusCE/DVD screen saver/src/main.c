/* Keep these headers */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

/* Standard headers (recommended) */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <graphx.h>

#include "gfx/image_gfx.h"
/* Put your function prototypes here */

/* Put all your globals here */

void main(void) {
	int
		sprX = 0,
		sprY = 0,
		sprXDir = 1,
		sprYDir = 1,
		colourIndex = 0;
	const int16_t colours[] = {0xFFE0, 0x0099, 0xF800, 0x07E0, 0xFC00};

	gfx_Begin();
	gfx_SetDrawBuffer();
	gfx_SetPalette(image_gfx_pal, sizeof_image_gfx_pal, 0);
	
	while(!os_GetCSC()) {
		gfx_ZeroScreen();
		gfx_Sprite(image, sprX, sprY);
		gfx_SwapDraw();
		sprX += sprXDir;
		sprY += sprYDir;

		(sprX == 0) && (sprXDir = 1);
		(sprY == 0) && (sprYDir = 1);
		(sprX == LCD_WIDTH - image_data[0]) && (sprXDir = -1);
		(sprY == LCD_HEIGHT - image_data[1]) && (sprYDir = -1);
		if((sprX < 2 || sprX + image_data[0] > LCD_WIDTH - 2) && (sprY < 2 || sprY + image_data[1] > LCD_HEIGHT - 2)) {
			image_gfx_pal[1] = colours[colourIndex = (colourIndex + 1) % 5];
			gfx_SetPalette(image_gfx_pal, sizeof_image_gfx_pal, 0);
		}
	}

	gfx_End();
}

/* Put other functions here */
