#include "main_menu.h"

#include "context.h"
#include "sounds.h"
#include "menu_draw.h"
#include "setup_menu.h"
#include "file_select.h"
#include "transition.h"

#define MAIN_MENU_LEN 3

void main_menu_init(struct Context* ctx) {
	ctx->menu.selected = 0;
	ctx->menu.len = MAIN_MENU_LEN;
	ctx->prog_state = PROG_MAIN_MENU;
}

void main_menu_loop(struct Menu* menu, struct Context* ctx) {
	menu_control(menu, ctx, ctx->input.up.just_pressed, ctx->input.down.just_pressed);

	if(ctx->input.select.just_pressed) {
		switch(menu->selected) {
    		case 0:
	            fade_transition(ctx, fselect_menu_init);
	            break;
	    	case 1:
	            fade_transition(ctx, setup_menu_init);
	            break;
	        case 2:
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

	draw_set_font(ctx, FONT_BIG);
	draw_text(ctx, "panelpon", -2, -12, PL_ALL_WHITE, JUSTIFY_L);

	draw_rect(ctx, (struct IRect){0, 0, 64, 56}, PL_ALL_WHITE);
	ctx->yoff += 8;

	const char* opt_strs[MAIN_MENU_LEN] = { "stor", "play", "exit" };
	menu_draw_list(ctx, menu, opt_strs);
}
