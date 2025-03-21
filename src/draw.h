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

	// Font context
	struct IRect font_src;

	// For changing color palletes during transition
	uint8_t transition_state;
};

enum Font {
	FONT_BIG,
	FONT_SMALL
};

enum Justification {
	JUSTIFY_L,
	JUSTIFY_C,
	JUSTIFY_R
};

void draw_clear(struct DrawContext* ctx);
void draw_present(struct DrawContext* ctx);

void draw_sprite(struct DrawContext* ctx, struct IRect src, int16_t x, int16_t y, struct Pallete pl);
void draw_sprite_flip(struct DrawContext* ctx, struct IRect src, int16_t x, int16_t y, struct Pallete pl, SDL_RendererFlip flip);
void draw_anim(struct DrawContext* ctx, uint8_t frame, uint8_t framelen, struct IRect src, int16_t x, int16_t y, struct Pallete pl);
void draw_anim_flip(struct DrawContext* ctx, uint8_t frame, uint8_t framelen, struct IRect src, int16_t x, int16_t y, struct Pallete pl, SDL_RendererFlip flip);
void draw_anim_flip_vert(struct DrawContext* ctx, uint8_t frame, uint8_t framelen, struct IRect src, int16_t x, int16_t y, struct Pallete pl, SDL_RendererFlip flip);
void draw_rect(struct DrawContext* ctx, struct IRect rect, struct Pallete pl);
void draw_fill_rect(struct DrawContext* ctx, struct IRect rect, struct Pallete pl);

void draw_set_font(struct DrawContext* ctx, enum Font font);
void draw_text(struct DrawContext* ctx, const char* str, int16_t x, int16_t y, struct Pallete pl, enum Justification justification);
struct Pallete fancy_blink_pl(uint8_t t, uint8_t rate_mod, uint8_t threshold, struct Pallete down, struct Pallete between, struct Pallete up);
struct Pallete pl_from_transition_state(struct Pallete pl, uint8_t state);

#endif // draw_h_INCLUDED
