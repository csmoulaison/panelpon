#include "menu_draw.h"

void menu_draw_list(struct DrawContext* ctx, struct Menu* menu, const char** opt_strs) {
	draw_set_font(ctx, FONT_BIG);

	for(int i = 0; i < menu->len; i++) {
		struct Pallete pl = PL_ALL_WHITE;
		uint8_t yoff = 0;
		if(i == menu->selected) {
    		// Fancy blinking text
			pl = PL_PURPLE;
        	uint8_t rem = (menu->clock / 16) % 8;
        	if(rem > 2) {
        		if(rem == 3 || rem == 7) {
            		yoff = 1;
        			pl = PL_PURPLE;
        		} else {
            		yoff = 2;
            		pl = PL_BLUE;
        		}
        	}
		}
		draw_text(ctx, opt_strs[i], 16, 1, PL_RED);
		draw_text(ctx, opt_strs[i], 16, 1 - yoff, pl);
		ctx->yoff += 16;
	}
}
