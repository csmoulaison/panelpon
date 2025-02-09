#ifndef game_h_INCLUDED
#define game_h_INCLUDED

#define MAX_TILE_EVENTS BOARD_LEN

#include "config.h"
#include "input.h"
#include "audio.h"

enum GameState {
    GAME_PRE,
    GAME_ACTIVE,
    GAME_POST
};

struct Game {
    // Misc game state
    enum GameState state;
	uint16_t timer;
	uint8_t yoff;
	uint8_t yoff_countdown;

	// Cursor, including movement animation state
    uint8_t cursor;
    uint8_t cursor_prev;
    uint8_t cursor_anim; // game frames elapsed - not (necesarily) anim frames

	// These indices all refer to the same board index, and, in all cases but
	// tiles[i] the value represents the number of game frames until the event is
	// complete - the value of tiles[i] refers to the type of tile stored.
	uint8_t tiles[BOARD_LEN];
	uint16_t explodes[BOARD_LEN];  // index references exploding tile, duh
	uint8_t flips[BOARD_LEN]; // index references left hand tile
	uint8_t falls[BOARD_LEN]; // index references end y pos
	uint8_t buf_falls[BOARD_LEN]; // index references start y pos
};

void game_init(struct Game* game);
void game_control(struct Game* game, struct Input* input, struct AudioContext* audio); 
void game_tick(struct Game* game, struct AudioContext* audio); 

#endif // game_h_INCLUDED
