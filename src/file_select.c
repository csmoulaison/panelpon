#include "file_select.h"

#include "context.h"
#include "sounds.h"
#include "menu_draw.h"
#include "new_game.h"
#include "transition.h"

#define MAX_SAVES 3

#define SAVEBOX_W 50
#define SAVEBOX_GAP 12
#define LEVEL_NUM 32

void fselect_menu_init(struct Context* ctx) {
	ctx->menu.selected = 0;
	ctx->menu.len = MAX_SAVES;

	ctx->prog_state = PROG_FSELECT;
}

void fselect_menu_loop(struct Menu* menu, struct Context* ctx) {
	if(ctx->input.quit.just_pressed) {
		fade_transition(ctx, main_menu_init);
	}

	menu_control(menu, ctx, ctx->input.left.just_pressed, ctx->input.right.just_pressed);

	if(ctx->input.select.just_pressed) {
		ctx->save.id = menu->selected;
		fade_transition(ctx, start_new_game);
	}
}

void fselect_menu_draw(struct Menu* menu, struct Game* game, struct DrawContext* ctx) {
	// NOTE from the amazing hannah baby
	// tiles fall within file border when it's selected, boop beep boop
    (void)menu;
	(void)game;

	ctx->xoff = LOGICAL_W / 2;
	ctx->yoff = LOGICAL_H / 2 - 30;

	draw_set_font(ctx, FONT_BIG);
	draw_text(ctx, "file select", 0, -12, PL_ALL_WHITE, JUSTIFY_C);

	ctx->xoff = LOGICAL_W / 2 - (SAVEBOX_W / 2) - SAVEBOX_W - SAVEBOX_GAP;

	for(uint8_t i = 0; i < MAX_SAVES; i++) {
    	struct Pallete pl1 = PL_ALL_WHITE;
    	struct Pallete pl2 = PL_ALL_WHITE;
    	if(i == menu->selected) {
	    	pl1 = fancy_blink_pl(menu->clock, 16, 3, PL_YELLOW, PL_PURPLE, PL_BLUE);
	    	pl2 = fancy_blink_pl(menu->clock, 8, 3, PL_ALL_BLACK, PL_BLUE, PL_YELLOW);
    	}

		int16_t xbox = i * (SAVEBOX_W + SAVEBOX_GAP); // nice
		draw_rect(ctx, (struct IRect){xbox, 0, SAVEBOX_W, 60}, pl1);

		draw_set_font(ctx, FONT_BIG);
		draw_text(ctx, "save", xbox + SAVEBOX_W / 2 + 1, 12, pl1, JUSTIFY_C);

		char savenum[2];
		sprintf(savenum, "%u", i + 1);
		draw_text(ctx, savenum, xbox + SAVEBOX_W / 2 + 1, 24, pl1, JUSTIFY_C);

		draw_set_font(ctx, FONT_SMALL);

		bool new_save = true;
		if(new_save) {
			draw_text(ctx, "new", xbox + SAVEBOX_W / 2 + 1, 40, pl2, JUSTIFY_C);
			draw_text(ctx, "game", xbox + SAVEBOX_W / 2 + 2, 46, pl2, JUSTIFY_C);
		} else {
			char progress[10];
			sprintf(progress, "%u-%u", 0, LEVEL_NUM);
			draw_text(ctx, progress, xbox + SAVEBOX_W / 2 + 1, 42, pl2, JUSTIFY_C);
		}
	}
}
