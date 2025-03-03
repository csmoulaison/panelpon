#include "cursor_hsplit.h"

#include "board.h"
#include "shift.h"
#include "cursor.h"

void cur_hsplit_init(struct Game* game) {
	game->cursor.pos = BOARD_LEN / 2 - 2;
}

bool cur_hsplit_shift(struct Game* game) {
	return swap_shift(game, game->cursor.pos, xoffset(game->cursor.pos, 2));
}

void cur_hsplit_draw(struct Game* game, struct DrawContext* ctx) {
	draw_cursor_box(ctx, &game->cursor, SPR_CURSOR_SQUARE, PL_ALL_WHITE, 1, 1, game->yoff);

	struct Cursor vcur;
	vcur.pos = xoffset(game->cursor.pos, 2);
	vcur.prev = xoffset(game->cursor.prev, 2);
	vcur.anim = game->cursor.anim;
	draw_cursor_box(ctx, &vcur, SPR_CURSOR_SQUARE, PL_ALL_WHITE, 1, 1, game->yoff);
}

bool cur_hsplit_move(struct Game* game, struct Input* input) {
	return cur_move_bounded(&game->cursor, input->up.just_pressed, input->down.just_pressed, input->left.just_pressed, input->right.just_pressed, BOARD_W - 3, BOARD_H - 2);
}
