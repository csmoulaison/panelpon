#ifndef draw_h_INCLUDED
#define draw_h_INCLUDED

#include "SDL2/SDL.h"
#include "stdint.h"
#include "stdbool.h"

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
};

void draw_clear(struct DrawContext* ctx);
void draw_present(struct DrawContext* ctx);

void draw_sprite(struct DrawContext* ctx, struct IRect src, uint8_t x, uint8_t y, struct Pallete pl);
void draw_sprite_flip(struct DrawContext* ctx, struct IRect src, uint8_t x, uint8_t y, struct Pallete pl, SDL_RendererFlip flip);
void draw_anim(struct DrawContext* ctx, float t, uint8_t frames, struct IRect src, uint8_t x, uint8_t y, struct Pallete pl);
void draw_anim_flip(struct DrawContext* ctx, float t, uint8_t frames, struct IRect src, uint8_t x, uint8_t y, struct Pallete pl, SDL_RendererFlip flip);

#endif // draw_h_INCLUDED
