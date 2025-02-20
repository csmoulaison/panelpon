#ifndef game_h_INCLUDED
#define game_h_INCLUDED

#define MAX_TILE_EVENTS BOARD_LEN

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
struct Swap {
    union {
        uint8_t positions[2];
        struct {
    		uint8_t a;
    		uint8_t b;
        };
    };
    uint8_t t;
};

struct Game {
    // Misc game state
    enum GameState state;
	uint8_t yoff;
	uint8_t yoff_countdown;
	bool grace_period;

	// Cursor, including movement animation state
    uint8_t cursor;
    uint8_t cursor_prev;
    uint8_t cursor_anim; // game frames elapsed - not (necesarily) anim frames

	// Function pointers associated with the cursor type chosen for the game.
	// Different cursor have different functionality with regards to movement,
	// appearance, and tile swapping.
	uint8_t (*cursor_start_pos)(struct Game* game);
	bool (*swap)(struct Game* game);
	void (*draw_cursor)(struct Game* game, struct DrawContext* draw);
	uint8_t (*move_cursor)(struct Game* game, struct Input* input);

	// These indices refer to positions on the game board and the values are the
	// type of tile at that position.
	uint8_t tiles[BOARD_LEN];

	// These indices are also board positions and the values represent the number
	// of ticks until the "event" associated with the position is finished.
	uint16_t matches  [BOARD_LEN];
	uint8_t  falls    [BOARD_LEN]; // index references end   (lower) y pos
	uint8_t  buf_falls[BOARD_LEN]; // index references start (upper) y pos

	// These indices aren't associated with board positions. This is just a list of
	// ongoing swaps.
	struct Swap swaps[BOARD_LEN];
	uint8_t swaps_len;
};

void game_init(struct Game* game);
void game_control(struct Game* game, struct Input* input, struct AudioContext* audio); 
void game_tick(struct Game* game, struct AudioContext* audio); 

bool tiles_reached_top(struct Game* game);

#endif // game_h_INCLUDED
