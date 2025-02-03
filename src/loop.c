#include "loop.h"

#include "match.h"

#include "stdio.h"
#include "stdlib.h"

void loop(Context* ctx) {
    // Calculate delta time
	ctx->time_last = ctx->time_now;	
	ctx->time_now = SDL_GetTicks64();
	float dt = (float)(ctx->time_now - ctx->time_last) / 1000;

	// Poll input
	input_poll(&ctx->input);

	// Simulate
	match_control(&ctx->match, &ctx->input);
	match_tick(&ctx->match, dt);
	match_draw(&ctx->match, &ctx->draw);

	/*if(ctx->input.select.just_pressed) {
		sound_play(&ctx->audio, &ctx->snd_test, &ctx->audio.data.voices[0]);
		ctx->snd_data = 400 + (rand() & 4) * 100;
	}*/

	// Draw
	draw_clear(&ctx->draw);
	match_draw(&ctx->match, &ctx->draw);
	draw_present(&ctx->draw);
}
