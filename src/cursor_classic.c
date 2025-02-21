#include "cursor_classic.h"

#include "board.h"
#include "shift.h"

uint8_t cur_classic_start_pos(struct Game* game) {
    (void)game;
	return BOARD_LEN / 2 - 1;
}

bool cur_classic_shift(struct Game* game) {
	uint8_t from = game->cursor;
	uint8_t to = xoffset(game->cursor, 1);

	if(!eligible_for_shift(game, from, to)) {
    	return false;
	}

	// Don't allow flip if a cursor tile is empty with a tile somewhere above it.
	for(int i = 0; i < 2; i++) {
    	uint8_t cur_check = xoffset(game->cursor, i);

    	if(!empty(game, cur_check)) {
        	continue;
    	}

		for(int j = 1; j < BOARD_H; j++) {
			// TODO - Change these conditions to use offset functions.
			if(cur_check / BOARD_W - j < 0) {
				break;
			}
			if(!empty(game, cur_check - j * BOARD_W)) {
				return false;
			}
		}
	}

	swap_tiles(game, from, to);

	// Do we actually need to cancel falls? Isn't this a bad scenario?
	// Result: seems not upon shallow examination
	//game->falls[right] = 0;
	//game->falls[game->cursor] = 0;

	// If we flip over an empty space, keep track of that.
	for(int i = 0; i < 2; i++) {
		if(empty(game, offset(game->cursor, i, 1))) {
			game->buf_falls[xoffset(game->cursor, i)] = FRAMES_SHIFT;
		}
	}

	return true;
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
	uint8_t curx = xcoord(game->cursor);
	uint8_t cury = ycoord(game->cursor);

   	if(input->up.just_pressed)    cury -= 1;
	if(input->down.just_pressed)  cury += 1;
	if(input->left.just_pressed)  curx -= 1;
	if(input->right.just_pressed) curx += 1;

	if(cury == 255)        cury = 0;
	if(curx == 255)        curx = 0;
	if(cury > BOARD_H - 2) cury = BOARD_H - 2;
	if(curx > BOARD_W - 2) curx = BOARD_W - 2; // -2 because cursor is 2x1

	return bindex(curx, cury);
}
