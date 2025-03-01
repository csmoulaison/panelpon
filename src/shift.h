#ifndef shift_h_INCLUDED
#define shift_h_INCLUDED

#include "game.h"
#include "stdbool.h"

bool eligible_for_shift(struct Game* game, uint8_t from, uint8_t to);
bool eligible_for_shift2(struct Game* game, uint8_t from, uint8_t to);
bool swap_tiles(struct Game* game, uint8_t from, uint8_t to);
bool shift_row(struct Game* game);

#endif // shift_h_INCLUDED
