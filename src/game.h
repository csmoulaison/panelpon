#ifndef game_h_INCLUDED
#define game_h_INCLUDED

#define MAX_TILE_EVENTS BOARD_LEN

#include "config.h"
#include "input.h"
#include "draw.h"
#include "audio.h"

struct Game {
    // Board, including y offset for upwards movement
	uint8_t board[BOARD_LEN];
	float yoff;
	float hitch;

	// Cursor, including movement animation state
    uint8_t cursor;
    uint8_t cursor_anim_prev;

    float cursor_anim_t;

	// Ongoing board events - some of these are gameplay significant, and some
	// are only for animation.
	// 
	// The indices directly equate to the board indices, and the value is the t
	// value between 0 and 1, > 1 signifying an inactive event.
	float flips[BOARD_LEN]; // index references left hand tile
	float explodes[BOARD_LEN];  // index references exploding tile, duh
	float falls[BOARD_LEN]; // index references end y pos
};

void game_control(struct Game* game, struct Input* input, struct AudioContext* audio); 
void game_tick(struct Game* game, struct AudioContext* audio, float dt); 
void game_draw(struct Game* game, struct DrawContext* ctx);
void game_update_matches(struct Game* game, struct AudioContext* audio);

void coords_from_index(uint8_t i, uint8_t* x, uint8_t* y);
uint8_t index_from_coords(uint8_t x, uint8_t y);
void spr_from_index(uint8_t* board, uint8_t i, struct IRect* spr, struct Pallete* pl);

#endif // game_h_INCLUDED
