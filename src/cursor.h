#ifndef cursor_h_INCLUDED
#define cursor_h_INCLUDED

#include "stdint.h"
#include "stdbool.h"

struct Cursor {
	uint8_t pos;
	uint8_t prev;
	uint8_t anim;
};

bool cur_move_bounded(struct Cursor* cursor, bool up, bool down, bool left, bool right, uint8_t xmax, uint8_t ymax);

#endif // cursor_h_INCLUDED
