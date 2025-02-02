#include "draw.h"

#include "stdio.h"

void draw_clear(DrawContext* ctx) {
	SDL_RenderClear(ctx->renderer);
}

void draw_present(DrawContext* ctx) {
	SDL_RenderPresent(ctx->renderer);
}

void draw_sprite(DrawContext* ctx, IRect src, IRect dst, Pallete pl) {
    draw_sprite_flip(ctx, src, dst, pl, false);
}

void draw_sprite_flip(DrawContext* ctx, IRect src, IRect dst, Pallete pl, bool flip) {
    uint8_t* c1 = (uint8_t*)&pl.primary;
    uint8_t* c2 = (uint8_t*)&pl.secondary;

    SDL_SetTextureColorMod(ctx->atlas_primary,   c1[0], c1[1], c1[2]);
    SDL_SetTextureColorMod(ctx->atlas_secondary, c2[0], c2[1], c2[2]);

	SDL_Rect s = (SDL_Rect){src.x, src.y, src.w, src.h};
	SDL_Rect d = (SDL_Rect){dst.x, dst.y, dst.w, dst.h};

    SDL_RenderCopyEx(ctx->renderer, ctx->atlas_primary,   &s, &d, 0, NULL, flip);
    SDL_RenderCopyEx(ctx->renderer, ctx->atlas_secondary, &s, &d, 0, NULL, flip);
}
