#include "loop.h"

#include "match.h"

#include "stdio.h"

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

	// Draw
	draw_clear(&ctx->draw);
	match_draw(&ctx->match, &ctx->draw);
	draw_present(&ctx->draw);
}
