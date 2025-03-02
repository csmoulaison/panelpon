#include "shift.h"

#include "board.h"

bool eligible_for_shift(struct Game* game, uint8_t from, uint8_t to) {
	bool both_empty = true;

	uint8_t positions[2] = {from, to};
    for(int i = 0; i < 2; i++) {
        uint8_t p = positions[i];

		if(fall_buffered(game, p) || matching(game, p) || falling(game, p)) {
			return false;
		}

		// Can't flip if either position is both not empty and has an empty position
		// below it. This removes the player's ability to shift more than one tile in
		// a column from below, ferrying a higher tile across a gap it shouldn't have
		// been able to cross.
		if(!empty(game, p) && empty(game, yoffset(p, 1))) {
			return false;
		}

		// Can't flip if either position is both empty and has any non-empty position
		// in the entire column of positions above it. This removes the player's
		// ability to shift more than one tile in a column from above, ferrying the
		// higher tile across a gap it shouldn't have been able to cross.
		if(empty(game, p)) {
			uint8_t x = xcoord(p);
			for(uint8_t y = ycoord(p - 1); y != 255; y--) {
				if(!empty(game, bindex(x, y))) {
					return false;
				}
			}
		} else {
    		both_empty = false;
		}
    }

	// Can't flip if both positions are empty.
	if(both_empty) {
		return false;
	} 
    
    return true;
}

// TODO - Obviously rename this. Really probably just inline it for now? See if
// it can be factored out of eligible_for_shift, but really might not be worth
// it in a way.
bool eligible_for_shift2(struct Game* game, uint8_t from, uint8_t to) {
	uint8_t positions[2] = {from, to};
    for(int i = 0; i < 2; i++) {
        uint8_t p = positions[i];
		if(fall_buffered(game, p) || matching(game, p) || falling(game, p)) {
			return false;
		}
    }
	return true;
}

bool swap_shift(struct Game* game, uint8_t from, uint8_t to) {
	if(!eligible_for_shift(game, from, to)) {
		return false;
	}
	
	// Swap tiles
	uint8_t tmp = game->tiles[from];
	game->tiles[from] = game->tiles[to];
	game->tiles[to] = tmp;

	// Cancel overlapping shifts
	for(int i = 0; i < BOARD_LEN; i++) {
		if(game->shifts[i].to_pos == from || game->shifts[i].to_pos == to) {
			game->shifts[i].t = 0;
		}
	}

	// Keep track of shifts.
	game->shifts[from].to_pos = to;
	game->shifts[to].to_pos   = from;

	uint8_t positions[2] = {to, from};
	for(int i = 0; i < 2; i++) {
		uint8_t p = positions[i];
		game->shifts[p].t = FRAMES_SHIFT;

		// Buffer a fall event if we have ended up over an empty space.
		if(empty(game, yoffset(p, 1))) {
			game->buf_falls[p] = FRAMES_SHIFT;
		}
	}

	return true;
}

bool cycle_shift(struct Game* game, uint8_t* positions, uint8_t len) {
	// Check for eligibility
	if(!eligible_for_shift2(game, positions[len - 1], positions[0])) {
		return false;
	}
	for(int i = 0; i < len - 1; i++) {
		if(!eligible_for_shift2(game, positions[i], positions[i + 1])) {
			return false;			
		}
	}

	// Shift tiles
	uint8_t last = positions[len - 1];
	// Confusingly, prev is first set to the last tile, so that it wraps.
	uint8_t prev = game->tiles[last]; 
	game->shifts[last].to_pos = positions[0];
	game->shifts[last].t = FRAMES_SHIFT;
	for(int i = 0; i < len; i++) {
		uint8_t p = positions[i];

		uint8_t tmp = prev;
		prev = game->tiles[p];

		game->tiles[p] = tmp;
		game->shifts[p].to_pos = positions[i + 1];
		game->shifts[p].t = FRAMES_SHIFT;

		if(empty(game, yoffset(p, 1))) {
			game->buf_falls[p] = FRAMES_SHIFT;
		}
	}

	return true;
}
