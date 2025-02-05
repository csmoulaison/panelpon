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
	if(ctx->game.hitch > -1) {
    	game_control(&ctx->game, &ctx->input, &ctx->audio);
    	game_tick(&ctx->game, &ctx->audio, dt);
    	game_draw(&ctx->game, &ctx->draw);
	} else if(ctx->input.select.just_pressed) {
    	ctx->game.hitch = 1;
	}

	// Audio
	audio_update(&ctx->audio);

	// Draw
	draw_clear(&ctx->draw);
	game_draw(&ctx->game, &ctx->draw);
	draw_present(&ctx->draw);
}
