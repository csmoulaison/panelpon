#ifndef draw_h_INCLUDED
#define draw_h_INCLUDED

#include "SDL2/SDL.h"
#include "stdint.h"
#include "stdbool.h"

#include "config.h"
#include "palletes.h"
#include "rect.h"

struct DrawContext {
    uint32_t screen_w;
    uint32_t screen_h;
    SDL_Texture* atlas_primary;
    SDL_Texture* atlas_secondary;
	SDL_Window* window;
	SDL_Renderer* renderer;
	uint32_t clear_color;

	uint8_t xoff;
	uint8_t yoff;
};

void draw_clear(struct DrawContext* ctx);
void draw_present(struct DrawContext* ctx);

void draw_sprite(struct DrawContext* ctx, struct IRect src, int16_t x, int16_t y, struct Pallete pl);
void draw_sprite_flip(struct DrawContext* ctx, struct IRect src, int16_t x, int16_t y, struct Pallete pl, SDL_RendererFlip flip);
void draw_anim(struct DrawContext* ctx, uint8_t frame, uint8_t framelen, struct IRect src, int16_t x, int16_t y, struct Pallete pl);
void draw_anim_flip(struct DrawContext* ctx, uint8_t frame, uint8_t framelen, struct IRect src, int16_t x, int16_t y, struct Pallete pl, SDL_RendererFlip flip);
void draw_rect(struct DrawContext* ctx, struct IRect rect, struct Pallete pl);
void draw_fill_rect(struct DrawContext* ctx, struct IRect rect, struct Pallete pl);

#endif // draw_h_INCLUDED
