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
	uint8_t curx = xcoord(game->cursor.pos) * 8;
	uint8_t cury = ycoord(game->cursor.pos) * 8 + 8 - game->yoff;

	draw_sprite(ctx, SPR_CURSOR_VBIAS, curx - 1, cury - 1, PL_ALL_WHITE);
	draw_sprite_flip(ctx, SPR_CURSOR_VBIAS, curx + 1, cury - 1, PL_ALL_WHITE, SDL_FLIP_HORIZONTAL);
	draw_sprite_flip(ctx, SPR_CURSOR_VBIAS, curx - 1, cury + 8 + 1, PL_ALL_WHITE, SDL_FLIP_VERTICAL);
	draw_sprite_flip(ctx, SPR_CURSOR_VBIAS, curx + 1, cury  + 8 + 1, PL_ALL_WHITE, SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
}

bool cur_vert_move(struct Game* game, struct Input* input) {
	return cur_move_bounded(&game->cursor, input->up.just_pressed, input->down.just_pressed, input->left.just_pressed, input->right.just_pressed, BOARD_W - 1, BOARD_H - 3);
}
