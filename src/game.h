#ifndef game_h_INCLUDED
#define game_h_INCLUDED

#define MAX_TILE_EVENTS BOARD_LEN
#define MAX_CURSORS 8
#include "cursor.h"
#include "config.h"
#include "input.h"
#include "audio.h"
#include "draw.h"

enum GameState {
    GAME_PRE,
    GAME_ACTIVE,
    GAME_POST
};

// Swaps occur between two different tiles and act as an "event" with a
// countdown timer (t) which functions identically to the values of explodes[],
// falls[], buf_falls[], etc.
struct Shift {
	uint8_t to_pos;
    uint8_t t;
};

struct Game {
    // Misc game state
    enum GameState state;
	uint8_t yoff;
	uint8_t yoff_countdown;
	bool grace_period;
	uint8_t cursors_len;

	// This union is mainly here for ergonomics, so we can refer to the "primary"
	// cursor as just "cursor", rather than "cursor[0]".
	union {
		struct Cursor cursors[MAX_CURSORS];
		struct Cursor cursor;
	};

	// Function pointers associated with the cursor type chosen for the game.
	// Different cursor have different functionality with regards to movement,
	// appearance, and tile swapping.
	void (*cursor_init)(struct Game* game);
	bool (*shift)(struct Game* game);
	void (*draw_cursor)(struct Game* game, struct DrawContext* draw);
	bool (*move_cursor)(struct Game* game, struct Input* input);

	// These indices refer to positions on the game board and the values are the
	// type of tile at that position.
	uint8_t tiles[BOARD_LEN];

	// These indices are also board positions and the values represent the number
	// of ticks until the "event" associated with the position is finished.
	uint16_t matches   [BOARD_LEN];
	uint8_t  falls     [BOARD_LEN]; // index references end   (lower) y pos
	uint8_t  buf_falls [BOARD_LEN]; // index references start (upper) y pos
	struct Shift shifts[BOARD_LEN]; // event timer encoded in shift.t
};

void game_init(struct Game* game);
void game_control(struct Game* game, struct Input* input, struct AudioContext* audio); 
void game_tick(struct Game* game, struct AudioContext* audio); 

bool tiles_reached_top(struct Game* game);

#endif // game_h_INCLUDED
