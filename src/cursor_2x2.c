#include "cursor_2x2.h"

#include "board.h"
#include "shift.h"
#include "cursor.h"

void cur_2x2_init(struct Game* game) {
	game->cursor.pos = BOARD_LEN / 2 - 1;
}

bool cur_2x2_shift(struct Game* game) {
	uint8_t positions[4] = {
		game->cursor.pos,
		xoffset(game->cursor.pos, 1),
		offset(game->cursor.pos, 1, 1),
		yoffset(game->cursor.pos, 1)
	};
	return cycle_shift(game, positions, 4);
}

void cur_2x2_draw(struct Game* game, struct DrawContext* ctx) {
	draw_cursor_box(ctx, &game->cursor, SPR_CURSOR_SQUARE, PL_ALL_WHITE, 2, 2, game->yoff);
}

bool cur_2x2_move(struct Game* game, struct Input* input) {
	return cur_move_bounded(&game->cursor, input->up.just_pressed, input->down.just_pressed, input->left.just_pressed, input->right.just_pressed, BOARD_W - 2, BOARD_H - 3);
}

