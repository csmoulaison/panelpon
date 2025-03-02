#ifndef shift_h_INCLUDED
#define shift_h_INCLUDED

#include "game.h"
#include "stdbool.h"

bool eligible_for_shift(struct Game* game, uint8_t from, uint8_t to);
bool eligible_for_shift2(struct Game* game, uint8_t from, uint8_t to);
bool swap_shift(struct Game* game, uint8_t from, uint8_t to);
bool cycle_shift(struct Game* game, uint8_t* positions, uint8_t len);

#endif // shift_h_INCLUDED
