#ifndef context_h_INCLUDED
#define context_h_INCLUDED

#include "SDL2/SDL.h"
#include "stdint.h"

#include "input.h"
#include "match.h"
#include "audio.h"

#define LOGICAL_W 216
#define LOGICAL_H 216

struct Context {
    // Time (for delta time calculation)
	uint64_t time_last;
	uint64_t time_now;

    struct DrawContext draw;
    struct AudioContext audio;
	struct Input input;
	struct Match match;
};

#endif // context_h_INCLUDED
