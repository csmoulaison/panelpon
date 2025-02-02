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

void match_tick(Match* match, double dt) {
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
}

void match_draw(Match* match, DrawContext* ctx) {
	uint8_t board_x = LOGICAL_W / 2 - BOARD_W / 2 * 8;
	uint8_t board_y = LOGICAL_H / 2 - BOARD_H / 2 * 8;

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

    	IRect left_spr;
    	Pallete left_pl;
    	spr_from_index(match->board, left, &left_spr, &left_pl);

    	left_spr.w = 16;
    	left_spr.x += 8 + (uint8_t)(3 * match->flips[i].t) * 16;
		draw_sprite_flip(ctx, left_spr, (IRect){board_x + flipx * 8, board_y + flipy * 8, 16, 8}, left_pl, true);

    	IRect right_spr;
    	Pallete right_pl;
    	spr_from_index(match->board, left + 1, &right_spr, &right_pl);

    	right_spr.w = 16;
    	right_spr.x += 8 + (uint8_t)(3 * match->flips[i].t) * 16;
		draw_sprite(ctx, right_spr, (IRect){board_x + flipx * 8, board_y + flipy * 8, 16, 8}, right_pl);

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
        draw_sprite(ctx, spr, (IRect){board_x + x * 8, board_y + y * 8, 8, 8}, pl);
	}

	// Cursor
	uint8_t curx;
	uint8_t cury;
	coords_from_index(match->cursor, &curx, &cury);
	draw_sprite(ctx, SPR_CURSOR, (IRect){board_x + curx * 8 - 1, board_y + cury * 8 - 1, 18, 10}, PL_ALL_WHITE);
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
