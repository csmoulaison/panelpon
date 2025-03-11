#include "loop.h"

#include "stdio.h"
#include "stdlib.h"
#include "time.h"

#include "game.h"
#include "sounds.h"
#include "setup_menu.h"
#include "file_select.h"

void loop(struct Context* ctx) {
    // Calculate delta time
	ctx->time_last = ctx->time_now;	
	ctx->time_now = SDL_GetPerformanceCounter();

	double frame_time = (double)(ctx->time_now - ctx->time_last) / (double)SDL_GetPerformanceFrequency();
	ctx->time_accumulator += frame_time;

	while(ctx->time_accumulator >= DT) {
		ctx->time_accumulator -= DT;

    	// Audio
    	audio_update(&ctx->audio);

    	// Poll input
    	input_poll(&ctx->input);

		// Do transition if active
    	if(ctx->transition_t != 0) {
			ctx->transition_t--;
	    	if(ctx->transition_t > 32) {
				ctx->draw.transition_state = 1;
				if(ctx->transition_t < 36) {
					ctx->draw.transition_state = 2;
				} 
				if(ctx->transition_t < 32) {
					ctx->draw.transition_state = 3;
				}
		    	continue;
	    	} else {
				ctx->draw.transition_state = 3;
				if(ctx->transition_t < 16) {
					ctx->draw.transition_state = 2;
				} 
				if(ctx->transition_t < 10) {
					ctx->draw.transition_state = 1;
				}
	    	}
	    	
			if(ctx->transition_t == 32) {
				ctx->transition_callback(ctx);
			}
			if(ctx->transition_t == 0) {
				ctx->draw.transition_state = 0;
			}
    	}

    	// Simulate
    	switch(ctx->prog_state) {
	    	case PROG_MAIN_MENU:
		    	main_menu_loop(&ctx->menu, ctx);
		    	break;
	    	case PROG_FSELECT:
		    	fselect_menu_loop(&ctx->menu, ctx);
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
    }

	// Draw
	draw_clear(&ctx->draw);
	switch(ctx->prog_state) {
		case PROG_MAIN_MENU:
			main_menu_draw(&ctx->menu, &ctx->draw);
			break;
		case PROG_FSELECT:
			fselect_menu_draw(&ctx->menu, &ctx->game, &ctx->draw);
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
