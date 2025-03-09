#include "menu.h"

#include "sounds.h"
#include "config.h"
#include "context.h"

void menu_control(struct Menu* menu, struct Context* ctx) {
	struct Input* input = &ctx->input;
	struct AudioContext* audio = &ctx->audio;
    
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
		menu->callback(menu, ctx);
	}
}

void menu_draw(struct Menu* menu, struct DrawContext* ctx) {
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
		draw_text(ctx, menu->options[i], 16, 0, pl);
		ctx->yoff += 16;
	}
}
