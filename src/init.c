#include "init.h"

#include "stdio.h"
#include "stdlib.h"

#include "config.h"

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
	{
    	DrawContext* draw = &ctx->draw;
    	SDL_DisplayMode mode;
    	SDL_GetDesktopDisplayMode(0, &mode);
    	draw->screen_w = mode.w;
    	draw->screen_h = mode.h;
    	
    	draw->window = SDL_CreateWindow("panelpon", 0, 0, draw->screen_w, draw->screen_h, SDL_WINDOW_BORDERLESS);
    	draw->renderer = SDL_CreateRenderer(draw->window, -1, SDL_RENDERER_ACCELERATED);
    	SDL_RenderSetLogicalSize(draw->renderer, LOGICAL_W, LOGICAL_H);

    	draw->atlas_primary = load_texture(draw->renderer, PATH_ATLAS_PRIMARY);
    	draw->atlas_secondary = load_texture(draw->renderer, PATH_ATLAS_SECONDARY);

    	SDL_Surface* icon= load_surface(PATH_ICON);
    	SDL_SetWindowIcon(draw->window, icon);
	}

	// Init input
	{
    	Input* input = &ctx->input;
        input->scancode_btn_maps_len = 0;
        input->mapped_btns_len = 0;

    	map_scancode_to_button(input, SDL_SCANCODE_ESCAPE, &input->quit);
    	map_scancode_to_button(input, SDL_SCANCODE_SPACE,  &input->select);
    	map_scancode_to_button(input, SDL_SCANCODE_RETURN, &input->select);
    	map_scancode_to_button(input, SDL_SCANCODE_Z,      &input->select);
    	map_scancode_to_button(input, SDL_SCANCODE_W, 	   &input->up);
    	map_scancode_to_button(input, SDL_SCANCODE_S, 	   &input->down);
    	map_scancode_to_button(input, SDL_SCANCODE_A, 	   &input->left);
    	map_scancode_to_button(input, SDL_SCANCODE_D, 	   &input->right);
    	map_scancode_to_button(input, SDL_SCANCODE_UP, 	   &input->up);
    	map_scancode_to_button(input, SDL_SCANCODE_DOWN,   &input->down);
    	map_scancode_to_button(input, SDL_SCANCODE_LEFT,   &input->left);
    	map_scancode_to_button(input, SDL_SCANCODE_RIGHT,  &input->right);
	}

	// Init match (this'll go somewhere else eventually, of course.
	{
    	Match* match = &ctx->match;
    	match->cursor = 0;
    	for(uint8_t i = 0; i < BOARD_LEN; i++) {
    		match->board[i] = rand() % (SHAPES_LEN + 1);
    		match->flips[i] = 2;
    		match->explodes[i] = 2;
    		match->falls[i] = 2;
    	}
	}
}


