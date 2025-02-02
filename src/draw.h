#ifndef draw_h_INCLUDED
#define draw_h_INCLUDED

#include "SDL2/SDL.h"
#include "stdint.h"
#include "stdbool.h"

#include "palletes.h"
#include "rect.h"

struct {
    uint32_t screen_w;
    uint32_t screen_h;
    SDL_Texture* atlas_primary;
    SDL_Texture* atlas_secondary;
	SDL_Window* window;
	SDL_Renderer* renderer;
	uint32_t clear_color;
} typedef DrawContext;

void draw_clear(DrawContext* ctx);
void draw_present(DrawContext* ctx);
void draw_sprite(DrawContext* ctx, IRect src, IRect dst, Pallete pallete);
void draw_sprite_flip(DrawContext* ctx, IRect src, IRect dst, Pallete pl, bool flip);

#endif // draw_h_INCLUDED
