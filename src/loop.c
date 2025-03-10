#include "loop.h"

#include "stdio.h"
#include "stdlib.h"
#include "time.h"

#include "game.h"
#include "sounds.h"
#include "setup_menu.h"

void loop(struct Context* ctx) {
    // Calculate delta time
	ctx->time_last = ctx->time_now;	
	ctx->time_now = SDL_GetPerformanceCounter();

	double frame_time = (double)(ctx->time_now - ctx->time_last) / (double)SDL_GetPerformanceFrequency();
	ctx->time_accumulator += frame_time;

	while(ctx->time_accumulator >= DT) {
		ctx->time_accumulator -= DT;

    	// Poll input
    	input_poll(&ctx->input);

    	// Simulate
    	switch(ctx->prog_state) {
	    	case PROG_MAIN_MENU:
		    	main_menu_loop(&ctx->menu, ctx);
		    	break;
	    	case PROG_SETUP:
		    	setup_menu_loop(&ctx->menu, ctx);
		    	break;
	    	case PROG_GAME:
				game_loop(ctx);
				break;
			default:
				break;
    	}
		
    	// Audio
    	audio_update(&ctx->audio);
    }

	// Draw
	draw_clear(&ctx->draw);
	switch(ctx->prog_state) {
		case PROG_MAIN_MENU:
			main_menu_draw(&ctx->menu, &ctx->draw);
			break;
		case PROG_SETUP:
			setup_menu_draw(&ctx->menu, &ctx->game, &ctx->draw);
			break;
		case PROG_GAME:
			game_draw(&ctx->game, &ctx->draw);
			break;
		default:
			break;
	}
	draw_present(&ctx->draw);
}
