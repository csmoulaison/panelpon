#include "loop.h"

#include "game.h"
#include "sounds.h"

#include "stdio.h"
#include "stdlib.h"
#include "time.h"

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
    	switch(ctx->game.state) {
        	case GAME_PRE:
            	if(ctx->input.select.just_pressed) {
                	ctx->game.state = GAME_ACTIVE;
                	struct Sound sound;
                	sound.priority = 1;
                	sound.callback = snd_move;
                	sound_play(&ctx->audio, sound);
            	}
            	break;
            case GAME_ACTIVE:
            	game_control(&ctx->game, &ctx->input, &ctx->audio);
            	game_tick(&ctx->game, &ctx->audio);
                break;
    		case GAME_POST:
        		ctx->game.timer -= 1;
            	if(ctx->input.select.just_pressed) {
                	game_init(&ctx->game);
                	struct Sound sound;
                	sound.priority = 1;
                	sound.callback = snd_move;
                	sound_play(&ctx->audio, sound);
            	}
        		break;
        	default:
            	break;
    	}

    	// Audio
    	audio_update(&ctx->audio);
    }

	// Draw
	draw_clear(&ctx->draw);
	game_draw(&ctx->game, &ctx->draw);
	draw_present(&ctx->draw);
}
