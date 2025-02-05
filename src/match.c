#include "match.h"

#include "stdbool.h"
#include "stdio.h"
#include "sounds.h"

void match_control(struct Match* match, struct Input* input, struct AudioContext* audio) {
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
    		match->cursor_anim_prev = match->cursor;
    		match->cursor_anim_t = 0;
    		match->cursor = cursor_new;

    		struct Sound sound;
    		sound.priority = 1;
    		sound.callback = snd_move;
    		sound_play(audio, sound);
    	}
    }

	// Flip tiles
	if(input->select.just_pressed) {
		struct Sound sound;
		sound.priority = 1;

    	if(match->board[match->cursor] == 0 && match->board[match->cursor + 1] == 0) {
    		sound.callback = snd_noflip;
    		sound_play(audio, sound);
        	goto endflip;
    	}

		sound.callback = snd_flip;
    	sound_play(audio, sound);

		uint8_t tmp = match->board[match->cursor];
		match->board[match->cursor] = match->board[match->cursor + 1];
		match->board[match->cursor + 1] = tmp;

		// Cancel adjacent flip animations
		if(curx > 0) {
       		match->flips[match->cursor + 1] = 2;
       		match->falls[match->cursor + 1] = 2;
		}
		if(curx < BOARD_W - 2) {
    		match->flips[match->cursor] = 2;
       		match->falls[match->cursor] = 2;
		}

		match->flips[match->cursor] = 0;
	}
endflip:
}

void match_tick(struct Match* match, float dt) {
	// Todo movements
	// - Tokens exploding - blocks until complete
	// - Token generation / board movement - constant, offset++ until tile sized

	// Check for tile falling
	for(uint8_t x = 0; x < BOARD_W; x++) {
    	for(uint8_t y = BOARD_H - 1; y >= 1; y--) {
        	uint8_t i = index_from_coords(x, y);

        	// Skip anything which isn't a non-empty tile on top of an empty tile,
        	// also excluding falls which are already in motion on the upper tile.
    		if(match->falls[i - BOARD_W] < 1 
    		|| match->flips[i - BOARD_W] < 1 
    		|| match->flips[i - BOARD_W - 1] < 1
    		|| match->board[i] != 0 
    		|| match->board[i - BOARD_W] == 0) {
				continue;
    		}

			match->board[i] = match->board[i - BOARD_W];
			match->board[i - BOARD_W] = 0;
			match->falls[i] = 0;
    	}
	}

	// Update event ticks
	for(uint8_t i = 0; i < BOARD_LEN; i++) {
		match->flips[i] += dt * 4;
		match->explodes[i] += dt * 8;
		match->falls[i] += dt * 6;
	}

	// Tick cursor
	match->cursor_anim_t += dt * 12;
}

void match_draw(struct Match* match, struct DrawContext* ctx) {
	uint8_t boardx = LOGICAL_W / 2 - BOARD_W / 2 * 8;
	uint8_t boardy = LOGICAL_H / 2 - BOARD_H / 2 * 8;

	// Skip these tiles during conventional drawing
	bool skips[BOARD_LEN];
	for(uint8_t i = 0; i < BOARD_LEN; i++) {
    	skips[i] = match->board[i] == 0;
	}

	// Draw flips
	for(uint8_t i = 0; i < BOARD_LEN; i++) {
		if(match->flips[i] > 0.5) {
    		continue;
		}
    	
    	// left and right refer to the new tile positions, which might be confusing,
    	// but reflects the current truth.
    	uint8_t flipx;
    	uint8_t flipy;
    	coords_from_index(i, &flipx, &flipy);

    	struct IRect spr;
    	struct Pallete pl;
		float anim_t = match->flips[i] * 2;

		if(match->board[i] != 0) {
        	spr_from_index(match->board, i, &spr, &pl);
        	spr.w *= 2;
        	spr.x += SPR_TILE_MOVE_OFFSET;
        	draw_anim_flip(ctx, anim_t, SPR_TILE_MOVE_FRAMES, spr, boardx + flipx * 8, boardy + flipy * 8, pl, true);
		}

		if(match->board[i + 1] != 0) {
        	spr_from_index(match->board, i + 1, &spr, &pl);
        	spr.w *= 2;
        	spr.x += SPR_TILE_MOVE_OFFSET;
        	draw_anim(ctx, anim_t, SPR_TILE_MOVE_FRAMES, spr, boardx + flipx * 8, boardy + flipy * 8, pl);
		}

    	skips[i] = true;
    	skips[i + 1] = true;
	}

	// Tiles
    for(uint8_t i = 0; i < BOARD_LEN; i++) {
		if(skips[i]) {
    		continue;
		}
        
    	struct IRect spr;
    	struct Pallete pl;
    	spr_from_index(match->board, i, &spr, &pl);

		uint8_t x;
		uint8_t y;
		coords_from_index(i, &x, &y);

		// Offset y if currently falling
		uint8_t yoff = 0;
        if(match->falls[i] < 1) {
            yoff = 8 * match->falls[i] - 8;
        }

        draw_sprite(ctx, spr, boardx + x * 8, boardy + y * 8 + yoff, pl);
	}

	// Cursor
	{
    	uint8_t curx;
    	uint8_t cury;
    	coords_from_index(match->cursor, &curx, &cury);
    	curx = boardx + curx * 8;
    	cury = boardy + cury * 8;
    	if(match->cursor_anim_t < 1) {
        	uint8_t frames = SPR_CURSOR_MOVE_FRAMES;
    		if(match->cursor_anim_prev == match->cursor + 1) {
    			struct IRect spr = SPR_CURSOR_MOVE_R;
    			draw_anim_flip(ctx, match->cursor_anim_t, frames, spr, curx - 2, cury - 1, PL_ALL_WHITE, true);
    		} else if(match->cursor_anim_prev == match->cursor - 1) {
				struct IRect spr = SPR_CURSOR_MOVE_R;
    			draw_anim(ctx, match->cursor_anim_t, frames, spr, curx - 9, cury - 1, PL_ALL_WHITE);
    		} else if(match->cursor_anim_prev == match->cursor - BOARD_W) {
				struct IRect spr = SPR_CURSOR_MOVE_D;
    			draw_anim(ctx, match->cursor_anim_t, frames, spr, curx - 1, cury - 9, PL_ALL_WHITE);
    		} else if(match->cursor_anim_prev == match->cursor + BOARD_W) {
				struct IRect spr = SPR_CURSOR_MOVE_D;
    			draw_anim_flip(ctx, match->cursor_anim_t, frames, spr, curx - 1, cury - 2, PL_ALL_WHITE, 2);
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

void spr_from_index(uint8_t* board, uint8_t i, struct IRect* spr, struct Pallete* pl) {
	switch(board[i]) {
        case 1:
            *spr = SPR_ROUND;
            *pl = PL_BLUE;
            break;
        case 2:
            *spr = SPR_SQUARE;
            *pl = PL_RED;
            break;
        case 3:
            *spr = SPR_DIAMOND;
            *pl = PL_GREEN;
            break;
        case 4:
            *spr = SPR_CROSS;
            *pl = PL_PURPLE;
            break;
        default:
            break;
	}
}
