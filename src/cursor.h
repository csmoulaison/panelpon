#ifndef cursor_h_INCLUDED
#define cursor_h_INCLUDED

#include "game.h"

uint8_t cur_move_bounded(struct Game* game, struct Input* input, uint8_t boundx, uint8_t boundy);

#endif // cursor_h_INCLUDED
