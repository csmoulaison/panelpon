#include "cursor_classic.h"

#include "board.h"

bool cur_classic_swap(struct Game* game) {
    // Check if we can flip
	if((empty(game, game->cursor) && empty(game, game->cursor + 1)) 
	|| fall_buffered(game, game->cursor) || fall_buffered(game, xoffset(game->cursor, 1))
	|| exploding(game, game->cursor) || exploding(game, xoffset(game->cursor, 1)) 
	|| falling(game, game->cursor)) {
    	return false;
	}

	// Don't allow flip if a cursor tile is empty with a tile somewhere above it.
	for(int i = 0; i < 2; i++) {
    	uint8_t check_cursor = xoffset(game->cursor, i);

    	if(!empty(game, check_cursor)) {
        	continue;
    	}

		for(int j = 1; j < BOARD_H; j++) {
			if(check_cursor / BOARD_W - j < 0) {
				break;
			}
			if(!empty(game, check_cursor - j * BOARD_W)) {
				return false;
			}
		}
	}

	// Only execute if we are able to flip
	uint8_t right = xoffset(game->cursor, 1);

	uint8_t tmp = game->tiles[game->cursor];
	game->tiles[game->cursor] = game->tiles[right];
	game->tiles[right] = tmp;

	// Cancel adjacent flip animations
	uint8_t curx = xcoord(game->cursor);
	if(curx > 0) {
   		game->flips[right] = 0;
   		game->falls[right] = 0;
	}
	if(curx < BOARD_W - 2) {
		game->flips[game->cursor] = 0;
   		game->falls[game->cursor] = 0;
	}
	game->flips[game->cursor] = FRAMES_FLIP;

	// If we flip over an empty space, keep track of that.
	for(int i = 0; i < 2; i++) {
		if(empty(game, game->cursor + i + BOARD_W)) {
			game->buf_falls[game->cursor + i] = FRAMES_FLIP;
		}
	}

	return true;
}

void cur_classic_draw(struct Game* game, struct DrawContext* ctx) {
	uint8_t curx = xcoord(game->cursor) * 8;
	uint8_t cury = ycoord(game->cursor) * 8 + 8 - game->yoff;

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
}
