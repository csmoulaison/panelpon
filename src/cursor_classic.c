#include "cursor_classic.h"

#include "board.h"
#include "shift.h"
#include "cursor.h"

void cur_classic_init(struct Game* game) {
	game->cursor.pos = BOARD_LEN / 2 - 1;
}

bool cur_classic_shift(struct Game* game) {
	return swap_tiles(game, game->cursor.pos, xoffset(game->cursor.pos, 1));
}

void cur_classic_draw(struct Game* game, struct DrawContext* ctx) {
	draw_cursor_box(ctx, &game->cursor, SPR_CURSOR_HBIAS, PL_ALL_WHITE, 2, 1, game->yoff);
}

bool cur_classic_move(struct Game* game, struct Input* input) {
	return cur_move_bounded(&game->cursor, input->up.just_pressed, input->down.just_pressed, input->left.just_pressed, input->right.just_pressed, BOARD_W - 2, BOARD_H - 2);
}
