#include "draw.h"

#include "stdio.h"

inline void draw_clear(struct DrawContext* ctx) {
	SDL_RenderClear(ctx->renderer);
}

inline void draw_present(struct DrawContext* ctx) {
	SDL_RenderPresent(ctx->renderer);
}

void draw_sprite(struct DrawContext* ctx, struct IRect src, uint8_t x, uint8_t y, struct Pallete pl) {
    draw_sprite_flip(ctx, src, x, y, pl, false);
}

void draw_sprite_flip(struct DrawContext* ctx, struct IRect src, uint8_t x, uint8_t y, struct Pallete pl, SDL_RendererFlip flip) {
    uint8_t* c1 = (uint8_t*)&pl.primary;
    uint8_t* c2 = (uint8_t*)&pl.secondary;

    SDL_SetTextureColorMod(ctx->atlas_primary,   c1[0], c1[1], c1[2]);
    SDL_SetTextureColorMod(ctx->atlas_secondary, c2[0], c2[1], c2[2]);

	SDL_Rect s = (SDL_Rect){src.x, src.y, src.w, src.h};
	SDL_Rect d = (SDL_Rect){x, y, src.w, src.h};

    SDL_RenderCopyEx(ctx->renderer, ctx->atlas_primary,   &s, &d, 0, NULL, flip);
    SDL_RenderCopyEx(ctx->renderer, ctx->atlas_secondary, &s, &d, 0, NULL, flip);
}

void draw_anim(struct DrawContext* ctx, float t, uint8_t frames, struct IRect src, uint8_t x, uint8_t y, struct Pallete pl) {
    draw_anim_flip(ctx, t, frames, src, x, y, pl, false);
}

void draw_anim_flip(struct DrawContext* ctx, float t, uint8_t frames, struct IRect src, uint8_t x, uint8_t y, struct Pallete pl, SDL_RendererFlip flip) {
    if(t >= 1) {
        t = 0.99;
    }
	src.x += (uint8_t)(frames * t) * src.w;
	draw_sprite_flip(ctx, src, x, y, pl, flip);
}

void draw_rect(struct DrawContext* ctx, struct IRect rect, struct Pallete pl) {
    uint8_t* c1 = (uint8_t*)&pl.primary;
    SDL_SetTextureColorMod(ctx->atlas_primary,   c1[0], c1[1], c1[2]);

	struct IRect src = SPR_PIXEL;
	SDL_Rect s = (SDL_Rect){src.x, src.y, src.w, src.h};
	SDL_Rect dleft = (SDL_Rect){rect.x, rect.y, 1, rect.h + 2};
	SDL_Rect dright = (SDL_Rect){rect.x + rect.w + 1, rect.y, 1, rect.h + 2};
	SDL_Rect dtop = (SDL_Rect){rect.x + 1, rect.y, rect.w, 1};
	SDL_Rect dbottom = (SDL_Rect){rect.x + 1, rect.y + rect.h + 1, rect.w, 1};

    SDL_RenderCopyEx(ctx->renderer, ctx->atlas_primary, &s, &dleft, 0, NULL, false);
    SDL_RenderCopyEx(ctx->renderer, ctx->atlas_primary, &s, &dright, 0, NULL, false);
    SDL_RenderCopyEx(ctx->renderer, ctx->atlas_primary, &s, &dtop, 0, NULL, false);
   	SDL_RenderCopyEx(ctx->renderer, ctx->atlas_primary, &s, &dbottom, 0, NULL, false);
}

void draw_fill_rect(struct DrawContext* ctx, struct IRect rect, struct Pallete pl) {
    uint8_t* c1 = (uint8_t*)&pl.primary;
    SDL_SetTextureColorMod(ctx->atlas_primary,   c1[0], c1[1], c1[2]);

	struct IRect src = SPR_PIXEL;
	SDL_Rect s = (SDL_Rect){src.x, src.y, src.w, src.h};
	SDL_Rect d = (SDL_Rect){rect.x + 1, rect.y + 1, rect.w - 2, rect.h - 2};

    SDL_RenderCopyEx(ctx->renderer, ctx->atlas_primary, &s, &d, 0, NULL, false);
}
