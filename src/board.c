#include "board.h"

// *** Tile state queries *** 

// Returns true if the given position is empty (tiles[i] == 0).
bool empty(struct Game* game, uint8_t i) {
	return game->tiles[i] == 0;
}

// Returns true if the given position is currently exploding (explodes[i] != 0).
bool exploding(struct Game* game, uint8_t i) {
	return game->explodes[i] != 0;
}

// Returns true if the given position is currently part of an ongoing flip. This
// checks both the left and right positions associated with the flip, with the
// left being the one stored in the flips[] array.
bool flipping(struct Game* game, uint8_t i) {
    bool flipping = game->flips[i] != 0;
    return flipping || (xcoord(i) != 0 && game->flips[xoffset(i, -1)] != 0);
}

// Returns true if the given position is currently part of an ongoing fall. This
// checks both the top and bottom positions associated with the fall, with the
// bottom being the one stored in the falls[] array.
bool falling(struct Game* game, uint8_t i) {
    bool falling = game->falls[i] != 0;
    return falling || (i / BOARD_W != 0 && game->falls[i - BOARD_W] != 0);
}

// 
bool fall_buffered(struct Game* game, uint8_t i) {
    return game->buf_falls[i] != 0;
}

// *** Index/coordinate converters ***

// Returns the position index from a given set of x,y coordinates.
uint8_t bindex(uint8_t x, uint8_t y) {
    return y * BOARD_W + x;
}

// Returns the x coordinate corresponding to a given position index.
uint8_t xcoord(uint8_t i) {
	return i % BOARD_W;
}

// Returns the y coordinate corresponding to a given position index.
uint8_t ycoord(uint8_t i) {
	return i / BOARD_W;
}

uint8_t offset(uint8_t i, uint8_t x, uint8_t y) {
	 return i + x + BOARD_W * y;
}

uint8_t xoffset(uint8_t i, uint8_t x) { 
	return i + x;
}

uint8_t yoffset(uint8_t i, uint8_t y) {
	return i + BOARD_W * y;
}
