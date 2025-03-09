#include "setup_menu.h"

#include "context.h"
#include "sounds.h"

void setup_menu_callback(struct Menu* menu, struct Context* ctx) {
	switch(menu->selection) {
    	case 0:
            ctx->prog_state = PROG_GAME;
            sound_play_new(&ctx->audio, snd_lose, 1, NULL);
            game_init(&ctx->game);
            break;
        case 1:
			ctx->prog_state = PROG_EXIT;
			break;
        default:
            break;
	}
}

