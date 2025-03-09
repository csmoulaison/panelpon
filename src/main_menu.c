#include "main_menu.h"

#include "sounds.h"
#include "config.h"
#include "prog_state.h"

enum ProgState menu_control(struct Menu* menu, struct Input* input, struct AudioContext* audio, struct Game* game) {
	bool moved = false;
	if(input->down.just_pressed) {
		moved = true;
		menu->selection++;
		if(menu->selection >= menu->len) {
			menu->selection = 0;
		}
	} else if(input->up.just_pressed) {
		moved = true;
		menu->selection--;
		if(menu->selection == 255) {
			menu->selection = menu->len - 1;
		}
	}

	if(moved) {
		sound_play_new(audio, snd_shift, 1, NULL);
	}

	if(input->select.just_pressed) {
		switch(menu->selection) {
			case 0:
				game_init(game);
				sound_play_new(audio, snd_match, 1, NULL);
				return PROG_GAME;
			case 1:
				return PROG_EXIT;
			default:
				break;
		}
	}

	return PROG_MAIN_MENU;
}

void menu_draw(struct Menu* menu, struct DrawContext* ctx) {
	const char* play = "play";
	const char* exit = "exit";
	const char* items[2] = {play, exit};

	ctx->xoff = LOGICAL_W / 2 - 32;
	ctx->yoff = LOGICAL_H / 2 - 20;

	draw_rect(ctx, (struct IRect){0, 0, 64, 40}, PL_ALL_WHITE);

	ctx->yoff += 8;
	draw_set_font(ctx, FONT_BIG);
	for(int i = 0; i < menu->len; i++) {
		struct Pallete pl = PL_ALL_WHITE;
		if(i == menu->selection) {
			pl = PL_BLUE;
		}
		draw_text(ctx, items[i], 16, 0, pl);
		ctx->yoff += 16;
	}
}
