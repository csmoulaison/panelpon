#include "main_menu.h"

#include "context.h"
#include "sounds.h"
#include "menu_draw.h"
#include "setup_menu.h"

void main_menu_init(struct Menu* menu) {
	menu->selected = 0;
	menu->len = 2;
}

void main_menu_loop(struct Menu* menu, struct Context* ctx) {
	menu_control(menu, ctx);

	if(ctx->input.select.just_pressed) {
		switch(menu->selected) {
	    	case 0:
	            ctx->prog_state = PROG_SETUP;
	            sound_play_new(&ctx->audio, snd_advance, 1, NULL);
	            setup_menu_init(menu, &ctx->game);
	            break;
	        case 1:
				ctx->prog_state = PROG_EXIT;
				break;
	        default:
	            break;
		}
	}
}

void main_menu_draw(struct Menu* menu, struct DrawContext* ctx) {
	ctx->xoff = LOGICAL_W / 2 - 32;
	ctx->yoff = LOGICAL_H / 2 - 20;
	draw_rect(ctx, (struct IRect){0, 1, 64, 38}, PL_ALL_WHITE);
	ctx->yoff += 8;

	const char* opt_strs[2] = { "play", "exit" };
	menu_draw_list(ctx, menu, opt_strs);
}
