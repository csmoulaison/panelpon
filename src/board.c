#include "board.h"

// Tile state queries
bool empty(struct Game* game, uint8_t i) {
	return game->tiles[i] == 0;
}

bool exploding(struct Game* game, uint8_t i) {
	return game->explodes[i] != 0;
}

bool flipping(struct Game* game, uint8_t i) {
    bool flipping = game->flips[i] != 0;
    return flipping || (i % BOARD_W != BOARD_W - 1 && game->flips[i - 1] != 0);
}

bool falling(struct Game* game, uint8_t i) {
    bool falling = game->falls[i] != 0;
    return falling || (i / BOARD_W != 0 && game->falls[i - BOARD_W] != 0);
}

bool fall_buffered(struct Game* game, uint8_t i) {
    return game->buf_falls[i] != 0;
}

// Index/coordinate converters
uint8_t bindex(uint8_t x, uint8_t y) {
    return y * BOARD_W + x;
}

uint8_t xcoord(uint8_t i) {
	return i % BOARD_W;
}

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
