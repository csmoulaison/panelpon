#ifndef context_h_INCLUDED
#define context_h_INCLUDED

#include "SDL2/SDL.h"
#include "stdint.h"

#include "prog_state.h"
#include "input.h"
#include "game.h"
#include "game_draw.h"
#include "audio.h"
#include "main_menu.h"

#define LOGICAL_W 216
#define LOGICAL_H 216

struct Context {
    // Time (for delta time calculation)
	uint64_t time_last;
	uint64_t time_now;
	double time_accumulator;

	enum ProgState prog_state;

	struct Input input;
    struct DrawContext draw;
    struct AudioContext audio;
	struct Game game;
	struct Menu menu;
};

#endif // context_h_INCLUDED
