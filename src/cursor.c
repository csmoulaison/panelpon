#include "cursor.h"

#include "board.h"

// Moves the cursor, clamping its coordinates between 0 and the given maximums.
bool cur_move_bounded(struct Cursor* cursor, bool up, bool down, bool left, bool right, uint8_t xmax, uint8_t ymax) {
	uint8_t curx = xcoord(cursor->pos);
	uint8_t cury = ycoord(cursor->pos);

   	if(up)    cury -= 1;
	if(down)  cury += 1;
	if(left)  curx -= 1;
	if(right) curx += 1;

	if(cury == 255) cury = 0;
	if(curx == 255) curx = 0;
	if(cury > ymax) cury = ymax;
	if(curx > xmax) curx = xmax; // -2 because cursor is 2x1

	cursor->prev = cursor->pos;
	cursor->pos = bindex(curx, cury);	
	return cursor->prev != cursor->pos;
}
