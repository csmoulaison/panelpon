#include "board.h"

// *** Tile state queries *** 

// Returns true if the given position is empty (tiles[pos] == 0).
bool empty(struct Game* game, uint8_t pos) {
	return game->tiles[pos] == 0;
}

// Returns true if the given position is currently exploding (explodes[pos] != 0).
bool matching(struct Game* game, uint8_t pos) {
	return game->matches[pos] != 0;
}

// Returns true if the given position is associated with an ongoing shift. This
// checks both the to and "from"(indexed) positions associated with every shift.
bool shifting(struct Game* game, uint8_t pos) {
    for(int i = 0; i < BOARD_LEN; i++) {
        if(game->shifts[i].t != 0 && (pos == i || game->shifts[pos].to_pos != i)) {
			return true;
        }
    }
    return false;
}

// Returns true if the given position is currently part of an ongoing fall. This
// checks both the top and bottom positions associated with the fall, with the
// bottom being the one stored in the falls[] array.
bool falling(struct Game* game, uint8_t pos) {
    bool falling = game->falls[pos] != 0;
    return falling || (pos / BOARD_W != 0 && game->falls[pos - BOARD_W] != 0);
}

// 
bool fall_buffered(struct Game* game, uint8_t pos) {
    return game->buf_falls[pos] != 0;
}

// *** Index/coordinate converters ***

// Returns the position index from a given set of x,y coordinates.
uint8_t bindex(uint8_t x, uint8_t y) {
    return y * BOARD_W + x;
}

// Returns the x coordinate corresponding to a given position index.
uint8_t xcoord(uint8_t pos) {
	return pos % BOARD_W;
}

// Returns the y coordinate corresponding to a given position index.
uint8_t ycoord(uint8_t pos) {
	return pos / BOARD_W;
}

uint8_t offset(uint8_t pos, uint8_t x, uint8_t y) {
	 return pos + x + BOARD_W * y;
}

uint8_t xoffset(uint8_t pos, uint8_t x) { 
	return pos + x;
}

uint8_t yoffset(uint8_t pos, uint8_t y) {
	return pos + BOARD_W * y;
}
