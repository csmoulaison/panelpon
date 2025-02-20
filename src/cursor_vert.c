#include "cursor_vert.h"

#include "board.h"

// TODO: Things to disallow:
// - 

uint8_t cur_vert_start_pos(struct Game* game) {
    (void)game;
	return BOARD_LEN / 2 - 1;
}

bool cur_vert_swap(struct Game* game) {
	// Changes to classic so far:
   	// - Position checking at start offset by y not x
    
    // Check if we can flip
	if((empty(game, game->cursor) && empty(game, yoffset(game->cursor, 1))) 
	|| fall_buffered(game, game->cursor) || fall_buffered(game, yoffset(game->cursor, 1))
	|| matching(game, game->cursor) || matching(game, yoffset(game->cursor, 1)) 
	|| falling(game, game->cursor)) {
    	return false;
	}

	// Don't allow flip if a cursor tile is empty with a tile somewhere above it.
	/* ? do we need this for vert?
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
	*/

	// Only execute if we are able to flip
	uint8_t down = yoffset(game->cursor, 1);

	uint8_t tmp = game->tiles[game->cursor];
	game->tiles[game->cursor] = game->tiles[down];
	game->tiles[down] = tmp;

	// Eliminate adjacent swaps
	uint8_t elims[game->swaps_len];
	uint8_t elims_len = 0;
	for(int i = 0; i < game->swaps_len; i++) {
    	uint8_t a = game->swaps[i].a;
    	uint8_t b = game->swaps[i].b;
		if(a == yoffset(game->cursor, -1) || a == game->cursor || a == down
		|| b == yoffset(game->cursor, -1) || b == game->cursor || b == down) {
			elims[elims_len] = i;
			elims_len++;
		}
	}
	for(int i = 0; i < elims_len; i++) {
		game->swaps[elims[i]] = game->swaps[game->swaps_len - 1];
		game->swaps_len--;
	}

	game->swaps[game->cursor].a = game->cursor;
	game->swaps[game->cursor].b = down;
	game->swaps[game->cursor].t = FRAMES_SWAP;

	// If we flip over an empty space, keep track of that.
	/* ? do we need this for vert?
	for(int i = 0; i < 2; i++) {
		if(empty(game, offset(game->cursor, i, 1)) {
			game->buf_falls[xoffset(game->cursor, i)] = FRAMES_FLIP;
		}
	}
	*/

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
	uint8_t curx = xcoord(game->cursor);
	uint8_t cury = ycoord(game->cursor);

   	if(input->up.just_pressed)    cury -= 1;
	if(input->down.just_pressed)  cury += 1;
	if(input->left.just_pressed)  curx -= 1;
	if(input->right.just_pressed) curx += 1;

	if(cury == 255)        cury = 0;
	if(curx == 255)        curx = 0;
	if(cury > BOARD_H - 3) cury = BOARD_H - 3;
	if(curx > BOARD_W - 1) curx = BOARD_W - 1; // -2 because cursor is 2x1

	return bindex(curx, cury);
}
