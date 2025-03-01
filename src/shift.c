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

bool swap_tiles(struct Game* game, uint8_t from, uint8_t to) {
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

bool shift_row(struct Game* game) {
	// Assuming cursor.pos is at x=0
	uint8_t cursor = game->cursor.pos;

	// Check for eligibility
	if(!eligible_for_shift2(game, xoffset(cursor, BOARD_W - 1), cursor)) {
		return false;
	}
	for(int i = 0; i < BOARD_W - 1; i++) {
		if(!eligible_for_shift2(game, xoffset(cursor, i), xoffset(cursor, i + 1))) {
			return false;			
		}
	}

	// Shift tiles
	uint8_t rightmost = xoffset(cursor, BOARD_W - 1);
	// Confusingly, left is first set to the rightmost tile, so that it wraps.
	uint8_t left = game->tiles[rightmost]; 
	game->shifts[rightmost].to_pos = cursor;
	game->shifts[rightmost].t = FRAMES_SHIFT;
	for(int i = 0; i < BOARD_W; i++) {
		uint8_t p = xoffset(cursor, i);

		uint8_t tmp = left;
		left = game->tiles[p];

		game->tiles[p] = tmp;
		game->shifts[p].to_pos = xoffset(cursor, i + 1);
		game->shifts[p].t = FRAMES_SHIFT;

		if(empty(game, yoffset(p, 1))) {
			game->buf_falls[p] = FRAMES_SHIFT;
		}
	}

	return true;
}
