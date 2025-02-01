#ifndef draw_h_INCLUDED
#define draw_h_INCLUDED

#include "SDL2/SDL.h"
#include "stdint.h"

#include "palletes.h"
#include "rect.h"

struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	uint32_t clear_color;
} typedef DrawContext;

DrawContext draw_context;

void draw_init();
void draw_clear();
void draw_present();
void draw_sprite(IRect src, IRect dst, Pallete pallete);

#endif // draw_h_INCLUDED
