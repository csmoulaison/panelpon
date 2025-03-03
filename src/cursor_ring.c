#include "cursor_ring.h"

#include "board.h"
#include "shift.h"
#include "cursor.h"

void cur_ring_init(struct Game* game) {
	game->cursor.pos = BOARD_LEN / 2 - 1;
}

bool cur_ring_shift(struct Game* game) {
	uint8_t p = game->cursor.pos;
	uint8_t positions[8] = {
		game->cursor.pos,
		xoffset(p, 1),
		xoffset(p, 2),
		offset(p, 2, 1),
		offset(p, 2, 2),
		offset(p, 1, 2),
		yoffset(p, 2),
		yoffset(p, 1)
	};
	return cycle_shift(game, positions, 8);
}

void cur_ring_draw(struct Game* game, struct DrawContext* ctx) {
	draw_cursor_box(ctx, &game->cursor, SPR_CURSOR_SQUARE, PL_ALL_WHITE, 3, 3, game->yoff);

	struct Cursor vcur;
	vcur.pos = offset(game->cursor.pos, 1, 1);
	vcur.prev = offset(game->cursor.prev, 1, 1);
	vcur.anim = game->cursor.anim;
	draw_cursor_box(ctx, &vcur, SPR_CURSOR_SQUARE, PL_ALL_WHITE, 1, 1, game->yoff);
}

bool cur_ring_move(struct Game* game, struct Input* input) {
	return cur_move_bounded(&game->cursor, input->up.just_pressed, input->down.just_pressed, input->left.just_pressed, input->right.just_pressed, BOARD_W - 3, BOARD_H - 4);
}

