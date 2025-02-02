#include "draw.h"

#include "stdio.h"

void draw_clear(DrawContext* ctx) {
	SDL_RenderClear(ctx->renderer);
}

void draw_present(DrawContext* ctx) {
	SDL_RenderPresent(ctx->renderer);
}

void draw_sprite(DrawContext* ctx, IRect src, uint8_t x, uint8_t y, Pallete pl) {
    draw_sprite_flip(ctx, src, x, y, pl, false);
}

void draw_sprite_flip(DrawContext* ctx, IRect src, uint8_t x, uint8_t y, Pallete pl, SDL_RendererFlip flip) {
    uint8_t* c1 = (uint8_t*)&pl.primary;
    uint8_t* c2 = (uint8_t*)&pl.secondary;

    SDL_SetTextureColorMod(ctx->atlas_primary,   c1[0], c1[1], c1[2]);
    SDL_SetTextureColorMod(ctx->atlas_secondary, c2[0], c2[1], c2[2]);

	SDL_Rect s = (SDL_Rect){src.x, src.y, src.w, src.h};
	SDL_Rect d = (SDL_Rect){x, y, src.w, src.h};

    SDL_RenderCopyEx(ctx->renderer, ctx->atlas_primary,   &s, &d, 0, NULL, flip);
    SDL_RenderCopyEx(ctx->renderer, ctx->atlas_secondary, &s, &d, 0, NULL, flip);
}

void draw_anim(DrawContext* ctx, float t, uint8_t frames, IRect src, uint8_t x, uint8_t y, Pallete pl) {
    draw_anim_flip(ctx, t, frames, src, x, y, pl, false);
}

void draw_anim_flip(DrawContext* ctx, float t, uint8_t frames, IRect src, uint8_t x, uint8_t y, Pallete pl, SDL_RendererFlip flip) {
	src.x += (uint8_t)(frames * t) * src.w;
	draw_sprite_flip(ctx, src, x, y, pl, flip);
}
