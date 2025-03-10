#include "file_select.h"

#include "context.h"
#include "sounds.h"
#include "menu_draw.h"

#define MAX_SAVES 3

void fselect_menu_init(struct Menu* menu, struct Game* game) {
    (void)game;
	menu->selected = 0;
	menu->len = MAX_SAVES;
}

void fselect_menu_loop(struct Menu* menu, struct Context* ctx) {
	if(ctx->input.quit.just_pressed) {
		ctx->prog_state = PROG_MAIN_MENU;
		main_menu_init(menu);
        sound_play_new(&ctx->audio, snd_back, 1, NULL);
	}

	menu_control(menu, ctx, ctx->input.left.just_pressed, ctx->input.right.just_pressed);
}

void fselect_menu_draw(struct Menu* menu, struct Game* game, struct DrawContext* ctx) {
	// NOTE from the amazing hannah baby
	// tiles fall within file border when it's selected, boop beep boop
    
    (void)menu;
	(void)game;

	ctx->xoff = LOGICAL_W / 2 - 74;
	ctx->yoff = LOGICAL_H / 2 - 30;

	for(int i = 0; i < MAX_SAVES; i++) {
    	struct Pallete pl = PL_ALL_WHITE;
    	if(i == menu->selected) {
        	pl = PL_RED;
    	}
		draw_rect(ctx, (struct IRect){i * 52, 0, 40, 60}, pl);
	}
}
