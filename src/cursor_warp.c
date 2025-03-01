#include "cursor_warp.h"

#include "board.h"
#include "shift.h"
#include "cursor.h"

void cur_warp_init(struct Game* game) {
	game->cursors[0].pos = BOARD_LEN / 2 - 1;
	game->cursors[1].pos = BOARD_LEN / 2;
}

bool cur_warp_shift(struct Game* game) {
	return shift_tiles(game, game->cursors[0].pos, game->cursors[1].pos);
}

void cur_warp_draw(struct Game* game, struct DrawContext* ctx) {
	struct Pallete pls[2] = {PL_BLUE, PL_RED};
	for(int i = 0; i < 2; i++) {
		draw_cursor_box(ctx, &game->cursors[i], SPR_CURSOR_SQUARE, pls[i], 1, 1, game->yoff);
	}
}

bool cur_warp_move(struct Game* game, struct Input* input) {
	return cur_move_bounded(&game->cursors[0], input->up.just_pressed, input->down.just_pressed, input->left.just_pressed, input->right.just_pressed, BOARD_W - 1, BOARD_H - 2)
		|| cur_move_bounded(&game->cursors[1], input->up2.just_pressed, input->down2.just_pressed, input->left2.just_pressed, input->right2.just_pressed, BOARD_W - 1, BOARD_H - 2);
}

