#include "loop.h"

void loop(Context* ctx) {
    // Calculate delta time
	ctx->time_last = ctx->time_now;	
	ctx->time_now = SDL_GetTicks64();
	ctx->dt = (double)(ctx->time_now - ctx->time_last) / 1000;

	// Poll input
	input_poll(&ctx->input);

	// Simulate and draw
	draw_clear(ctx);

	// ... simulate game loop ...
	
	draw_present(ctx);
}
