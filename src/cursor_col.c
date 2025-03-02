#include "cursor_col.h"

#include "board.h"
#include "shift.h"
#include "cursor.h"

// TODO - This one sucks as is. The falling from the top always delays the move.
// Any salvaging it?

void cur_col_init(struct Game* game) {
	game->cursor.pos = BOARD_H / 2 - 1;
}

bool cur_col_shift(struct Game* game) {
	uint8_t positions[BOARD_H];
	for(int i = 0; i < BOARD_H; i++) {
		positions[i] = yoffset(game->cursor.pos, i);
	}
	return cycle_shift(game, positions, BOARD_H);
}

void cur_col_draw(struct Game* game, struct DrawContext* ctx) {
	draw_cursor_box(ctx, &game->cursor, SPR_CURSOR_VBIAS, PL_ALL_WHITE, 1, BOARD_H - 1, game->yoff);
}

bool cur_col_move(struct Game* game, struct Input* input) {
	return cur_move_bounded(&game->cursor, false, false, input->left.just_pressed, input->right.just_pressed, BOARD_W - 1, 1);
}

