#include "cursor.h"

#include "board.h"

uint8_t cur_move_bounded(struct Game* game, struct Input* input, uint8_t boundx, uint8_t boundy) {
	uint8_t curx = xcoord(game->cursor);
	uint8_t cury = ycoord(game->cursor);

   	if(input->up.just_pressed)    cury -= 1;
	if(input->down.just_pressed)  cury += 1;
	if(input->left.just_pressed)  curx -= 1;
	if(input->right.just_pressed) curx += 1;

	if(cury == 255)        cury = 0;
	if(curx == 255)        curx = 0;
	if(cury > boundy) cury = boundy;
	if(curx > boundx) curx = boundx; // -2 because cursor is 2x1

	return bindex(curx, cury);
}
