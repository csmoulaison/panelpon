#ifndef context_h_INCLUDED
#define context_h_INCLUDED

#include "SDL2/SDL.h"
#include "stdint.h"

#include "input.h"
#include "match.h"

#define LOGICAL_W 216
#define LOGICAL_H 216

struct {
    // Time (for delta time calculation)
	uint64_t time_last;
	uint64_t time_now;
	double dt;
    
    // Windows, renderers, and textures. Oh, my!
    uint32_t screen_w;
    uint32_t screen_h;
    SDL_Texture* atlas_primary;
    SDL_Texture* atlas_secondary;
	SDL_Window* window;
	SDL_Renderer* renderer;
	uint32_t clear_color;

	// Input state. Might have multiple if we implement two player mode.
	Input input;

	// Game state
	Match match;
} typedef Context;

#endif // context_h_INCLUDED
