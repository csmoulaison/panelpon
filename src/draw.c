#include "draw.h"

void draw_init() {
	draw_context.window = SDL_CreateWindow("panelpon", 0, 0, 0, 0, SDL_WINDOW_SHOWN);
	draw_context.renderer = SDL_CreateRenderer(draw_context.window, -1, 0);
}

void draw_clear() {
	SDL_RenderClear(draw_context.renderer);
}

void draw_present() {
	SDL_RenderPresent(draw_context.renderer);
}

void draw_sprite(IRect src, IRect dst, Pallete pallete) {

}
