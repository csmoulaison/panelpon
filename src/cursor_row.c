#include "cursor_row.h"

#include "board.h"
#include "shift.h"
#include "cursor.h"

void cur_row_init(struct Game* game) {
	game->cursor.pos = bindex(0, BOARD_H / 2);
}

bool cur_row_shift(struct Game* game) {
	return shift_row(game);
}

void cur_row_draw(struct Game* game, struct DrawContext* ctx) {
	draw_cursor_box(ctx, &game->cursor, SPR_CURSOR_HBIAS, PL_ALL_WHITE, BOARD_W, 1, game->yoff);
}

bool cur_row_move(struct Game* game, struct Input* input) {
	return cur_move_bounded(&game->cursor, input->up.just_pressed, input->down.just_pressed, false, false, 1, BOARD_H - 2);
}
