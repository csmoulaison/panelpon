#include "cursor_classic.h"

#include "board.h"
#include "shift.h"
#include "cursor.h"

uint8_t cur_classic_start_pos(struct Game* game) {
    (void)game;
	return BOARD_LEN / 2 - 1;
}

bool cur_classic_shift(struct Game* game) {
	return shift_tiles(game, game->cursor, xoffset(game->cursor, 1));
}

void cur_classic_draw(struct Game* game, struct DrawContext* ctx) {
	uint8_t curx = xcoord(game->cursor) * 8;
	uint8_t cury = ycoord(game->cursor) * 8 + 8 - game->yoff;

	draw_sprite(ctx, SPR_CURSOR_HBIAS, curx - 1, cury - 1, PL_ALL_WHITE);
	draw_sprite_flip(ctx, SPR_CURSOR_HBIAS, curx + 8 + 1, cury - 1, PL_ALL_WHITE, SDL_FLIP_HORIZONTAL);
	draw_sprite_flip(ctx, SPR_CURSOR_HBIAS, curx - 1, cury + 1, PL_ALL_WHITE, SDL_FLIP_VERTICAL);
	draw_sprite_flip(ctx, SPR_CURSOR_HBIAS, curx + 8 + 1, cury + 1, PL_ALL_WHITE, SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);

	/* animations
	if(game->cursor_anim != FRAMES_CURSOR) {
    	uint8_t framelen = SPR_CURSOR_MOVE_FRAME_LEN;
		if(game->cursor_prev == game->cursor + 1) {
			struct IRect spr = SPR_CURSOR_MOVE_R;
			draw_anim_flip(ctx, game->cursor_anim, framelen, spr, curx - 2, cury - 1, PL_ALL_WHITE, true);
		} else if(game->cursor_prev == game->cursor - 1) {
			struct IRect spr = SPR_CURSOR_MOVE_R;
			draw_anim(ctx, game->cursor_anim, framelen, spr, curx - 9, cury - 1, PL_ALL_WHITE);
		} else if(game->cursor_prev == game->cursor - BOARD_W) {
			struct IRect spr = SPR_CURSOR_MOVE_D;
			draw_anim(ctx, game->cursor_anim, framelen, spr, curx - 1, cury - 9, PL_ALL_WHITE);
		} else if(game->cursor_prev == game->cursor + BOARD_W) {
			struct IRect spr = SPR_CURSOR_MOVE_D;
			draw_anim_flip(ctx, game->cursor_anim, framelen, spr, curx - 1, cury - 2, PL_ALL_WHITE, 2);
		}
	} else {
    	draw_sprite(ctx, SPR_CURSOR, curx - 1, cury - 1, PL_ALL_WHITE);
	}
	*/
}

uint8_t cur_classic_move(struct Game* game, struct Input* input) {
	return cur_move_bounded(game, input, BOARD_W - 2, BOARD_H - 2);
}
