#include "cursor_vert.h"

#include "board.h"
#include "shift.h"
#include "cursor.h"

void cur_vert_init(struct Game* game) {
	game->cursor.pos = BOARD_LEN / 2 - 1;
}

bool cur_vert_shift(struct Game* game) {
	return shift_tiles(game, game->cursor.pos, yoffset(game->cursor.pos, 1));
}

void cur_vert_draw(struct Game* game, struct DrawContext* ctx) {
	draw_cursor_box(ctx, &game->cursor, SPR_CURSOR_VBIAS, PL_ALL_WHITE, 1, 2, game->yoff);
	return;
}

bool cur_vert_move(struct Game* game, struct Input* input) {
	return cur_move_bounded(&game->cursor, input->up.just_pressed, input->down.just_pressed, input->left.just_pressed, input->right.just_pressed, BOARD_W - 1, BOARD_H - 3);
}
