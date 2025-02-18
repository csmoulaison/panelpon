#ifndef cursor_common_h_INCLUDED
#define cursor_common_h_INCLUDED

#include "game.h"
#include "stdbool.h"

bool cur_common_can_flip(struct Game* game, uint8_t* positions, uint8_t positions_len);

#endif // cursor_common_h_INCLUDED
