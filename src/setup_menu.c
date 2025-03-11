#include "setup_menu.h"

#include "context.h"
#include "sounds.h"
#include "menu_draw.h"
#include "speed_lut.h"

#define SETUP_OPTIONS_LEN 4

enum SetupOption {
	SETOPT_CURSOR,
	SETOPT_SPEED,
	SETOPT_INCR,
	SETOPT_START
};

void setup_menu_init(struct Menu* menu, struct Game* game) {
	game->cursor_type = CUR_CLASSIC;
	game->start_speed= 0;
	game->speed_increase_interval = 0;

	menu->selected = 0;
	menu->len = SETUP_OPTIONS_LEN;
}

void setup_menu_loop(struct Menu* menu, struct Context* ctx) {
	if(ctx->input.quit.just_pressed) {
		ctx->prog_state = PROG_MAIN_MENU;
		main_menu_init(menu);
        sound_play_new(&ctx->audio, snd_back, 1, NULL);
	}
	
	menu_control(menu, ctx, ctx->input.up.just_pressed, ctx->input.down.just_pressed);

	// Get value to modify via the logic below ...
	uint8_t* value;
	// Keep in mind min_val might be the uint8_t max (255). A bit counterintuitive,
	// but these represent the index at which the logic wraps, in other words, they
	// represent an exclusive range.
	uint8_t min_val; 
	uint8_t max_val;
	switch(menu->selected) {
		case SETOPT_CURSOR:
			value = (uint8_t*)&ctx->game.cursor_type;
			min_val = 0;
			max_val = CURSOR_TYPE_LEN;
			break;
		case SETOPT_SPEED:
			// Remember game.start_speed is a lookup index, not a meaningful value.
			value = (uint8_t*)&ctx->game.start_speed;
			min_val = 255;
			max_val = SPEEDS_LEN;
			break;
		case SETOPT_INCR:
			// Remember game.start_speed is a lookup index, not a meaningful value.
			value = (uint8_t*)&ctx->game.speed_increase_interval;
			min_val = 255;
			max_val = 11;
			break;
		default:
			break;
	}

	// ... The aformentioned logic: wrap the number within an exclusive range.
	bool changed = false; // just for sound
	if(menu->selected == 0 || menu->selected == 1 || menu->selected == 2) {
		if(ctx->input.left.just_pressed) {
			*value -= 1;
			if(*value == min_val) {
				*value = max_val - 1;
			}
			changed = true;
		}

		if(ctx->input.right.just_pressed) {
			*value += 1;
			if(*value == max_val) {
				*value = min_val + 1;
			}
			changed = true;
		}
	}
	if(changed) {
	    sound_play_new(&ctx->audio, snd_shift, 1, NULL);
	}

	if(ctx->input.select.just_pressed) {
		switch(menu->selected) {
	    	case SETOPT_START:
	            ctx->prog_state = PROG_GAME;
	            sound_play_new(&ctx->audio, snd_advance, 1, NULL);
	            game_init(&ctx->game);
	            break;
	        default:
	            break;
		}
	}
}

void setup_menu_draw(struct Menu* menu, struct Game* game, struct DrawContext* ctx) {
	ctx->xoff = LOGICAL_W / 2 - 64;
	ctx->yoff = LOGICAL_H / 2 - 40;
	draw_rect(ctx, (struct IRect){0, 1, 128, 78}, PL_ALL_WHITE);

	draw_set_font(ctx, FONT_SMALL);
	draw_text(ctx, "game setup", 64, -7, PL_ALL_WHITE, JUSTIFY_C);

	ctx->yoff += 12;
	char cursor_str[8];
	switch(game->cursor_type) {
		case CUR_CLASSIC:
			sprintf(cursor_str, "classic");
			break;
		case CUR_VERT:
			sprintf(cursor_str, "vert");
			break;
		case CUR_WARP:
			sprintf(cursor_str, "warp");
			break;
		case CUR_ROW:
			sprintf(cursor_str, "row");
			break;
		case CUR_COL:
			sprintf(cursor_str, "column");
			break;
		case CUR_SNAKE:
			sprintf(cursor_str, "snake");
			break;
		case CUR_RING:
			sprintf(cursor_str, "ring");
			break;
		case CUR_TWOBYTWO:
			sprintf(cursor_str, "2x2");
			break;
		case CUR_HSPLIT:
			sprintf(cursor_str, "split");
			break;
		default:
			sprintf(cursor_str, "error");
			break;
	}

	(void)speeds; // ignore unused variable warning
	char speed_str[10];
	sprintf(speed_str, "speed %u", game->start_speed+ 1);

	char incr_str[10];
	sprintf(incr_str, "incr %u", game->speed_increase_interval);

	const char* opt_strs[SETUP_OPTIONS_LEN] = { cursor_str, speed_str, incr_str, "start" };
	menu_draw_list(ctx, menu, opt_strs);
}
