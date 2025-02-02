#include "match.h"

#include "stdbool.h"
#include "stdio.h"

void match_control(Match* match, Input* input) {
    // Updated and used throughout function
	uint8_t curx;
	uint8_t cury;
	coords_from_index(match->cursor, &curx, &cury);

    // Move cursor
    {
       	if(input->up.just_pressed)    cury -= 1;
    	if(input->down.just_pressed)  cury += 1;
    	if(input->left.just_pressed)  curx -= 1;
    	if(input->right.just_pressed) curx += 1;

    	if(cury == 255)        cury = 0;
    	if(curx == 255)        curx = 0;
    	if(cury > BOARD_H - 1) cury = BOARD_H - 1;
    	if(curx > BOARD_W - 2) curx = BOARD_W - 2; // -2 because cursor is 2x1

    	uint8_t cursor_new = index_from_coords(curx, cury);
		// Keep track of the previous cursor position for animation
    	if(cursor_new != match->cursor) {
    		match->cursor_prev.tile = match->cursor;
    		match->cursor_prev.t = 0;
    		match->cursor = cursor_new;
    	}
    }

	// Flip tiles
	if(input->select.just_pressed) {
		uint8_t tmp = match->board[match->cursor];
		match->board[match->cursor] = match->board[match->cursor + 1];
		match->board[match->cursor + 1] = tmp;

		// Cancel adjacent flip animations
		for(int i = 0; i < match->flips_len; i++) {
			bool equal = match->cursor == match->flips[i].tile;
			bool right = curx > 0 && match->cursor + 1 == match->flips[i].tile;
			bool left =  curx < BOARD_W - 2 && match->cursor == match->flips[i].tile + 1;

			if(equal || right || left) {
    			match->flips[i].t = 1;
			}
		}

		match->flips[match->flips_len] = (TileEvent){match->cursor, 0};
		match->flips_len++;
	}
}

void match_tick(Match* match, float dt) {
	// Types of movement
	// - Token flipping - purely cosmetic, functionally instantaneous
	// - Token falling - blocks associated token movements until complete
	// - Tokens exploding - blocks until complete
	// - Cursor movement (all 4) - cosmetic, functionally instantaneous
	// - Token generation / board movement - constant, offset++ until tile sized

	// Tick flips
	for(int i = 0; i < match->flips_len; i++) {
		match->flips[i].t += dt * 8;
		while(match->flips[i].t >= 1) {
    		match->flips_len--;
			if(i == match->flips_len) {
    			// Need a goto if we do anything after this while loop
    			break;
			}
    		
    		match->flips[i] = match->flips[match->flips_len];
    		match->flips[i].t += dt;
		}
	}

	// Tick cursor
	if(match->cursor_prev.t < 1) {
		match->cursor_prev.t += dt * 12;
	}
}

void match_draw(Match* match, DrawContext* ctx) {
	uint8_t boardx = LOGICAL_W / 2 - BOARD_W / 2 * 8;
	uint8_t boardy = LOGICAL_H / 2 - BOARD_H / 2 * 8;

	// Skip these tiles during conventional drawing
	bool skips[BOARD_LEN];
	for(int i = 0; i < BOARD_LEN; i++) {
    	skips[i] = false;
	}

	// Draw flips
	for(uint8_t i = 0; i < match->flips_len; i++) {
    	// left and right refer to the new tile positions, which might be confusing,
    	// but reflects the current truth.
		uint8_t left = match->flips[i].tile;
    	
    	uint8_t flipx;
    	uint8_t flipy;
    	coords_from_index(left, &flipx, &flipy);

    	IRect spr;
    	Pallete pl;

    	spr_from_index(match->board, left, &spr, &pl);
    	spr.w = 16;
    	spr.x += 8;
    	draw_anim_flip(ctx, match->flips[i].t, 3, spr, boardx + flipx * 8, boardy + flipy * 8, pl, true);

    	spr_from_index(match->board, left + 1, &spr, &pl);
    	spr.w = 16;
    	spr.x += 8;
    	draw_anim(ctx, match->flips[i].t, 3, spr, boardx + flipx * 8, boardy + flipy * 8, pl);

    	skips[left] = true;
    	skips[left + 1] = true;
	}

	// Tiles
    for(uint8_t i = 0; i < BOARD_LEN; i++) {
		if(skips[i]) {
    		continue;
		}
        
    	IRect spr;
    	Pallete pl;
    	spr_from_index(match->board, i, &spr, &pl);

		uint8_t x;
		uint8_t y;
		coords_from_index(i, &x, &y);
        draw_sprite(ctx, spr, boardx + x * 8, boardy + y * 8, pl);
	}

	// Cursor
	{
    	uint8_t curx;
    	uint8_t cury;
    	coords_from_index(match->cursor, &curx, &cury);
    	curx = boardx + curx * 8;
    	cury = boardy + cury * 8;
    	if(match->cursor_prev.t < 1) {
    		if(match->cursor_prev.tile == match->cursor + 1) {
    			IRect spr = SPR_CURSOR_MOVE_R;
    			draw_anim_flip(ctx, match->cursor_prev.t, 2, spr, curx - 2, cury - 1, PL_ALL_WHITE, true);
    		} else if(match->cursor_prev.tile == match->cursor - 1) {
				IRect spr = SPR_CURSOR_MOVE_R;
    			draw_anim(ctx, match->cursor_prev.t, 2, spr, curx - 9, cury - 1, PL_ALL_WHITE);
    		} else if(match->cursor_prev.tile == match->cursor - BOARD_W) {
				IRect spr = SPR_CURSOR_MOVE_D;
    			draw_anim(ctx, match->cursor_prev.t, 2, spr, curx - 1, cury - 9, PL_ALL_WHITE);
    		} else if(match->cursor_prev.tile == match->cursor + BOARD_W) {
				IRect spr = SPR_CURSOR_MOVE_D;
    			draw_anim_flip(ctx, match->cursor_prev.t, 2, spr, curx - 1, cury - 2, PL_ALL_WHITE, 2);
    		}
    	} else {
        	draw_sprite(ctx, SPR_CURSOR, curx - 1, cury - 1, PL_ALL_WHITE);
    	}
	}
}

void coords_from_index(uint8_t i, uint8_t* x, uint8_t* y) {
	*x = i % BOARD_W;
	*y = i / BOARD_W;
}

uint8_t index_from_coords(uint8_t x, uint8_t y) {
    return y * BOARD_W + x;
}

void spr_from_index(uint8_t* board, uint8_t i, IRect* spr, Pallete* pl) {
	switch(board[i]) {
        case 0:
            *spr = SPR_ROUND;
            *pl = PL_BLUE;
            break;
        case 1:
            *spr = SPR_SQUARE;
            *pl = PL_RED;
            break;
        case 2:
            *spr = SPR_DIAMOND;
            *pl = PL_GREEN;
            break;
        case 3:
            *spr = SPR_CROSS;
            *pl = PL_PURPLE;
            break;
        default:
            break;
	}
}
