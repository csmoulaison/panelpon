#ifndef board_h_INCLUDED
#define board_h_INCLUDED

#include "game.h"

bool empty(struct Game* game, uint8_t i);
bool exploding(struct Game* game, uint8_t i);
bool flipping(struct Game* game, uint8_t i);
bool falling(struct Game* game, uint8_t i);
bool fall_buffered(struct Game* game, uint8_t i);

uint8_t bindex(uint8_t x, uint8_t y);
uint8_t xcoord(uint8_t i);
uint8_t ycoord(uint8_t i);
uint8_t offset(uint8_t i, uint8_t x, uint8_t y);
uint8_t xoffset(uint8_t i, uint8_t x);
uint8_t yoffset(uint8_t i, uint8_t x);

#endif // board_h_INCLUDED
