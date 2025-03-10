#include "menu_draw.h"

void menu_draw_list(struct DrawContext* ctx, struct Menu* menu, const char** opt_strs) {
	draw_set_font(ctx, FONT_BIG);

	for(int i = 0; i < menu->len; i++) {
		struct Pallete pl = PL_ALL_WHITE;
		if(i == menu->selected) {
			pl = PL_BLUE;
		}
		draw_text(ctx, opt_strs[i], 16, 0, pl);
		ctx->yoff += 16;
	}
}
