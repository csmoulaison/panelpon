#include "init.h"
#include "stdio.h"
#include "stdlib.h"

#define PATH_ICON "icon.bmp"
#define PATH_ATLAS_PRIMARY "atlas_primary.bmp"
#define PATH_ATLAS_SECONDARY "atlas_secondary.bmp"

#define LOGICAL_W 216
#define LOGICAL_H 216
#define SHAPES_LEN 4

SDL_Surface* load_surface(const char* path) {
	SDL_Surface* surf = SDL_LoadBMP(path);
	if(!surf) {
    	printf("Error loading SDL_Surface.\n");
    	exit(1);
	}
	uint32_t key = SDL_MapRGB(surf->format, 0, 0, 0);
	SDL_SetColorKey(surf, 1, key);
	return surf;
}

SDL_Texture* load_texture(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* surf = load_surface(path);
	return SDL_CreateTextureFromSurface(renderer, surf);
}

void init(Context* ctx) {
	// Init SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	// Init time
	ctx->time_now = SDL_GetTicks64();
	ctx->time_last = 0;
    
	// Init draw context
	SDL_DisplayMode mode;
	SDL_GetDesktopDisplayMode(0, &mode);
	ctx->screen_w = mode.w;
	ctx->screen_h = mode.h;
	
	ctx->window = SDL_CreateWindow("panelpon", 0, 0, ctx->screen_w, ctx->screen_h, SDL_WINDOW_BORDERLESS);
	ctx->renderer = SDL_CreateRenderer(ctx->window, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderSetLogicalSize(ctx->renderer, LOGICAL_W, LOGICAL_H);

	ctx->atlas_primary = load_texture(ctx->renderer, PATH_ATLAS_PRIMARY);
	ctx->atlas_secondary = load_texture(ctx->renderer, PATH_ATLAS_SECONDARY);

	SDL_Surface* icon= load_surface(PATH_ICON);
	SDL_SetWindowIcon(ctx->window, icon);

	// Init input
    ctx->input.scancode_btn_maps_len = 0;
    ctx->input.mapped_btns_len = 0;

	map_scancode_to_button(&ctx->input, SDL_SCANCODE_ESCAPE, &ctx->input.quit);
	map_scancode_to_button(&ctx->input, SDL_SCANCODE_SPACE,  &ctx->input.select);
	map_scancode_to_button(&ctx->input, SDL_SCANCODE_W, 	 &ctx->input.up);
	map_scancode_to_button(&ctx->input, SDL_SCANCODE_S, 	 &ctx->input.down);
	map_scancode_to_button(&ctx->input, SDL_SCANCODE_A, 	 &ctx->input.left);
	map_scancode_to_button(&ctx->input, SDL_SCANCODE_D, 	 &ctx->input.right);

	// Init match (this'll go somewhere else eventually, of course.
	ctx->match.curx = 0;
	ctx->match.cury = 0;
	for(uint8_t i = 0; i < BOARD_LEN; i++) {
		ctx->match.board[i] = rand() % SHAPES_LEN;
	}
}

