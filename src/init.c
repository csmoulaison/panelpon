#include "init.h"

#include "stdio.h"
#include "stdlib.h"
#include "time.h"

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

void init(struct Context* ctx) {
	// Init SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	// Init rand
	srand(time(NULL));
    
	// Init draw context
	{
    	struct DrawContext* draw = &ctx->draw;
    	SDL_DisplayMode mode;
    	SDL_GetDesktopDisplayMode(0, &mode);
    	draw->screen_w = mode.w;
    	draw->screen_h = mode.h;
    	
    	//draw->window = SDL_CreateWindow("panelpon", 0, 0, draw->screen_w, draw->screen_h, 0);
    	draw->window = SDL_CreateWindow("panelpon", 0, 0, 1024, 1024, SDL_WINDOW_RESIZABLE);
    	draw->renderer = SDL_CreateRenderer(draw->window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    	SDL_RenderSetLogicalSize(draw->renderer, LOGICAL_W, LOGICAL_H);

    	draw->atlas_primary = load_texture(draw->renderer, PATH_ATLAS_PRIMARY);
    	draw->atlas_secondary = load_texture(draw->renderer, PATH_ATLAS_SECONDARY);

    	SDL_Surface* icon= load_surface(PATH_ICON);
    	SDL_SetWindowIcon(draw->window, icon);
	}

	// Init audio context
	{
		struct AudioContext* audio = &ctx->audio;
		for(int i = 0; i < VOICES_LEN; i++) {
			audio->voices[i].soundstack_len = 0;
		}

    	PaError e = Pa_Initialize();
    	if(e != paNoError) {
    		printf("PortAudio error: %s\n", Pa_GetErrorText(e));
    		exit(1);
    	}

    	PaStream* stream;
    	e = Pa_OpenDefaultStream(&stream, 0, 1, paFloat32, SAMPLE_RATE, 128, audio_callback, &ctx->audio);
    	if(e != paNoError) {
    		printf("PortAudio error: %s\n", Pa_GetErrorText(e));
    		exit(1);
    	}

    	e = Pa_StartStream(stream);
    	if(e != paNoError) {
    		printf("PortAudio error: %s\n", Pa_GetErrorText(e));
    		exit(1);
    	}

	}

	// Init input
	{
    	struct Input* input = &ctx->input;
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

    	input->quit_event = false;
    	for(int i = 0; i < input->mapped_btns_len; i++) {
			input->mapped_btns[i]->just_pressed = false;
			input->mapped_btns[i]->just_released = false;
			input->mapped_btns[i]->held = false;
    	}
	}

	game_init(&ctx->game);

	// Init time
	ctx->time_now = SDL_GetPerformanceCounter();
	ctx->time_last = 0;
	ctx->time_accumulator = 0;
}
