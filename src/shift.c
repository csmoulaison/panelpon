#include "shift.h"

#include "board.h"

bool eligible_for_shift(struct Game* game, uint8_t from, uint8_t to) {
	bool all_empty = true;

	uint8_t positions[2] = {from, to};
    for(int i = 0; i < 2; i++) {
        uint8_t p = positions[i];
		if(fall_buffered(game, p) || matching(game, p) || falling(game, p)) {
			return false;
		}
		if(!empty(game, p)) {
    		all_empty = false;
		}
    }

	if(all_empty) {
		return false;
	} 
    
    return true;
}

void swap_tiles(struct Game* game, uint8_t from, uint8_t to) {
	// Swap tiles
	uint8_t tmp = game->tiles[from];
	game->tiles[from] = game->tiles[to];
	game->tiles[to] = tmp;

	// Cancel overlapping shifts
	game->shifts[from].t = 0;
	game->shifts[to].t = 0;
	for(int i = 0; i < BOARD_LEN; i++) {
		if(game->shifts[i].to_pos == from || game->shifts[i].to_pos == to) {
			game->shifts[i].t = 0;
		}
	}

	// Keep track of shifts
	game->shifts[from].t = FRAMES_SHIFT;
	game->shifts[to].t   = FRAMES_SHIFT;

	game->shifts[from].to_pos = to;
	game->shifts[to].to_pos   = from;
}
