#include "init.h"
#include "stdio.h"
#include "stdlib.h"

#define PATH_ICON "icon.bmp"
#define PATH_ATLAS_PRIMARY "atlas_primary.bmp"
#define PATH_ATLAS_SECONDARY "atlas_secondary.bmp"

SDL_Surface* load_surface(const char* path) {
	SDL_Surface* surf = SDL_LoadBMP(path);
	if(!surf) {
    	printf("Error loading SDL_Surface.\n");
    	exit(1);
	}
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
	ctx->window = SDL_CreateWindow("panelpon", 0, 0, 0, 0, SDL_WINDOW_SHOWN);
	ctx->renderer = SDL_CreateRenderer(ctx->window, -1, 0);

	ctx->atlas_primary = load_texture(ctx->renderer, PATH_ATLAS_PRIMARY);
	ctx->atlas_secondary = load_texture(ctx->renderer, PATH_ATLAS_SECONDARY);

	SDL_Surface* icon= load_surface(PATH_ICON);
	SDL_SetWindowIcon(ctx->window, icon);

	// Init input
    ctx->input.scancode_btn_maps_len = 0;
    ctx->input.mapped_btns_len = 0;

	map_scancode_to_button(&ctx->input, SDL_SCANCODE_ESCAPE, &ctx->input.quit);
	map_scancode_to_button(&ctx->input, SDL_SCANCODE_W, &ctx->input.up);
	map_scancode_to_button(&ctx->input, SDL_SCANCODE_S, &ctx->input.down);
	map_scancode_to_button(&ctx->input, SDL_SCANCODE_A, &ctx->input.left);
	map_scancode_to_button(&ctx->input, SDL_SCANCODE_D, &ctx->input.right);
	map_scancode_to_button(&ctx->input, SDL_SCANCODE_SPACE, &ctx->input.select);
}

