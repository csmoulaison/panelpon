#include "loop.h"

#include "game.h"

#include "stdio.h"
#include "stdlib.h"

void loop(struct Context* ctx) {
    // Calculate delta time
	ctx->time_last = ctx->time_now;	
	ctx->time_now = SDL_GetTicks64();
	float dt = (float)(ctx->time_now - ctx->time_last) / 1000;

	// Poll input
	input_poll(&ctx->input);

	// Simulate
	switch(ctx->game.state) {
    	case GAME_PRE:
        	if(ctx->input.select.just_pressed) {
            	ctx->game.state = GAME_ACTIVE;
        	}
        	break;
        case GAME_ACTIVE:
        	game_control(&ctx->game, &ctx->input, &ctx->audio);
        	game_tick(&ctx->game, &ctx->audio, dt);
            break;
		case GAME_POST:
    		ctx->game.hitch += dt;
        	if(ctx->input.select.just_pressed) {
            	game_init(&ctx->game);
        	}
    		break;
    	default:
        	break;
	}

	// Audio
	audio_update(&ctx->audio);

	// Draw
	draw_clear(&ctx->draw);
	game_draw(&ctx->game, &ctx->draw);
	draw_present(&ctx->draw);
}
