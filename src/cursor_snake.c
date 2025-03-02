#include "cursor_snake.h"

#include "board.h"
#include "shift.h"
#include "cursor.h"

void cur_snake_init(struct Game* game) {
	game->cursors_len = 2;
	for(int i = 0; i < game->cursors_len; i++) {
		game->cursors[i].pos = yoffset(BOARD_LEN / 2, -i);
		//game->cursors[i].pos = BOARD_LEN / 2;
	}
}

bool cur_snake_shift(struct Game* game) {
	uint8_t positions[game->cursors_len];
	for(int i = 0; i < game->cursors_len; i++) {
		positions[i] = game->cursors[game->cursors_len - 1 - i].pos;
	}
	return cycle_shift(game, positions, game->cursors_len);
}

void cur_snake_draw(struct Game* game, struct DrawContext* ctx) {
	struct Pallete pl = PL_BLUE;
	for(int i = 0; i < game->cursors_len; i++) {
		if(i == 1) {
			pl = PL_ALL_WHITE;
		}
		draw_cursor_box(ctx, &game->cursors[i], SPR_CURSOR_SQUARE, pl, 1, 1, game->yoff);
	}
}

bool cur_snake_move(struct Game* game, struct Input* input) {
	struct Cursor tmp = game->cursor;
	bool moved = cur_move_bounded(&game->cursor, input->up.just_pressed, input->down.just_pressed, input->left.just_pressed, input->right.just_pressed, BOARD_W - 1, BOARD_H - 2);
	if(moved) {
		// Stop snake from eating itself
		uint8_t mod = 1;
		if(game->cursors_len == 2) {
			mod = 0;
		}
		// Doesn't check against tail, UNLESS we are only 2 long (per mod above)
		for(int i = 1; i < game->cursors_len - mod; i++) {
			if(game->cursor.pos == game->cursors[i].pos) {
				game->cursor = tmp;
				return false;
			}
		}
		for(int i = 1; i < game->cursors_len; i++) {
			game->cursors[i].anim = 0;
			game->cursors[i].prev = game->cursors[i].pos;
			game->cursors[i].pos = tmp.pos;
			tmp.pos = game->cursors[i].prev;
		}
	}
	return moved;
}
