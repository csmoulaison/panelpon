#include "cursor_vert.h"

#include "board.h"
#include "shift.h"
#include "cursor.h"

uint8_t cur_vert_start_pos(struct Game* game) {
    (void)game;
	return BOARD_LEN / 2 - 1;
}

bool cur_vert_shift(struct Game* game) {
	// TODO - Disallow "ferrying" tiles upwards.
	uint8_t from = game->cursor;
	uint8_t to = yoffset(game->cursor, 1);
		
	if(!eligible_for_shift(game, from, to)) {
    	return false;
	}

	swap_tiles(game, from, to);
	return true;
}

void cur_vert_draw(struct Game* game, struct DrawContext* ctx) {
	uint8_t curx = xcoord(game->cursor) * 8;
	uint8_t cury = ycoord(game->cursor) * 8 + 8 - game->yoff;

	draw_sprite(ctx, SPR_CURSOR_VBIAS, curx - 1, cury - 1, PL_ALL_WHITE);
	draw_sprite_flip(ctx, SPR_CURSOR_VBIAS, curx + 1, cury - 1, PL_ALL_WHITE, SDL_FLIP_HORIZONTAL);
	draw_sprite_flip(ctx, SPR_CURSOR_VBIAS, curx - 1, cury + 8 + 1, PL_ALL_WHITE, SDL_FLIP_VERTICAL);
	draw_sprite_flip(ctx, SPR_CURSOR_VBIAS, curx + 1, cury  + 8 + 1, PL_ALL_WHITE, SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
}

uint8_t cur_vert_move(struct Game* game, struct Input* input) {
	return cur_move_bounded(game, input, BOARD_W - 1, BOARD_H - 3);
}
