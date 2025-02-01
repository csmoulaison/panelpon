#include "draw.h"


void draw_clear(Context* ctx) {
	SDL_RenderClear(ctx->renderer);
}

void draw_present(Context* ctx) {
	SDL_RenderPresent(ctx->renderer);
}

void draw_sprite(Context* ctx, IRect src, IRect dst, Pallete pallete) {

}
