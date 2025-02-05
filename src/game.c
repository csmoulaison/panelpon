#include "game.h"

#include "stdbool.h"
#include "stdio.h"
#include "sounds.h"

// TODO: Fixes/improvements
// - Block flips on: falls, explodes
// - Ignore matches on: falls, explodes - hmm, same thing
// - Prevent gaps from being crossed - really, just need to make sure that if a
//   flip completes with an empty tile below, there is no logic which has any
//   time to act on that tile - in other words, put the tile in a falling state
//   at the same site as its flip completes, unless you can determine it will
//   get caught at the right moment, and maintainably so!
//     (This also prevents flips over gaps which immediately result in a match.
// - Implement priority system to sounds so we can at least see what a less
//   chaotic soundscape would be like. Kinda cool as is, though. Kinda fucked.
// - How to signal that the bottom row is unselectable? All grey? Inverse? Kinda
//   still needs to be clear what type of tile the thing is.
// - Make a tile for each 255 combo. (255, 255, 0), (255, 0, 255), (0, 255, 255)

void game_control(struct Game* game, struct Input* input, struct AudioContext* audio) {
    // Updated and used throughout function
	uint8_t curx;
	uint8_t cury;
	coords_from_index(game->cursor, &curx, &cury);

    // Move cursor
    {
       	if(input->up.just_pressed)    cury -= 1;
    	if(input->down.just_pressed)  cury += 1;
    	if(input->left.just_pressed)  curx -= 1;
    	if(input->right.just_pressed) curx += 1;

    	if(cury == 255)        cury = 0;
    	if(curx == 255)        curx = 0;
    	if(cury > BOARD_H - 2) cury = BOARD_H - 2;
    	if(curx > BOARD_W - 2) curx = BOARD_W - 2; // -2 because cursor is 2x1

    	uint8_t cursor_new = index_from_coords(curx, cury);
		// Keep track of the previous cursor position for animation
    	if(cursor_new != game->cursor) {
    		game->cursor_anim_prev = game->cursor;
    		game->cursor_anim_t = 0;
    		game->cursor = cursor_new;

			// Probably only play if no match
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

    	if((game->board[game->cursor] == 0 && game->board[game->cursor + 1] == 0)
		|| game->explodes[game->cursor] < 1 || game->explodes[game->cursor + 1] < 1) {
    		sound.callback = snd_noflip;
    		sound_play(audio, sound);
        	goto endflip;
    	}

		sound.callback = snd_flip;
    	sound_play(audio, sound);

		uint8_t tmp = game->board[game->cursor];
		game->board[game->cursor] = game->board[game->cursor + 1];
		game->board[game->cursor + 1] = tmp;

		// Cancel adjacent flip animations
		if(curx > 0) {
       		game->flips[game->cursor + 1] = 2;
       		game->falls[game->cursor + 1] = 2;
		}
		if(curx < BOARD_W - 2) {
    		game->flips[game->cursor] = 2;
       		game->falls[game->cursor] = 2;
		}

		game->flips[game->cursor] = 0;

	}

endflip:
}

void game_tick(struct Game* game, struct AudioContext* audio, float dt) {
	bool update_matches = false;

	// Move board up, generating new tiles as needed
	game->hitch += dt;
	if(game->hitch < 1) {
		goto postmove;
	}
	
	game->yoff -= dt * 4;
	if(game->yoff < 0) {
		game->yoff += 8;

		// Move everything up one tile, including events
		if(game->cursor >= BOARD_W) {
			game->cursor -= BOARD_W;
		}
		for(int y = 1; y < BOARD_H; y++) {
			for(int x = 0; x < BOARD_W; x++) {
    			uint8_t oldpos = y * BOARD_W + x;
    			uint8_t newpos = oldpos - BOARD_W;

				game->board[newpos]    = game->board[oldpos];
				game->flips[newpos]    = game->flips[oldpos];
				game->explodes[newpos] = game->explodes[oldpos];
				game->falls[newpos]    = game->falls[oldpos];
			}
		}
		for(int x = 0; x < BOARD_W; x++) {
    		game->board[(BOARD_H - 1) * BOARD_W + x] = 1 + rand() % (SHAPES_LEN);
		}

		update_matches = true;
	}

postmove:

	// Check for tile falling
	for(uint8_t x = 0; x < BOARD_W; x++) {
    	for(uint8_t y = BOARD_H - 1; y >= 1; y--) {
        	uint8_t i = index_from_coords(x, y);

        	// Skip anything which isn't a non-empty tile on top of an empty tile,
        	// also excluding falls which are already in motion on the upper tile.
    		if(game->falls[i - BOARD_W]     < 1 
    		|| game->flips[i - BOARD_W]     < 1 
    		|| game->flips[i - BOARD_W - 1] < 1
    		|| game->board[i]           != 0 
    		|| game->board[i - BOARD_W] == 0) {
				continue;
    		}

			game->board[i] = game->board[i - BOARD_W];
			game->board[i - BOARD_W] = 0;
			game->falls[i] = 0;
    	}
	}

	// Update event ticks
	for(uint8_t i = 0; i < BOARD_LEN; i++) {
    	float prev_explode = game->explodes[i];
		game->explodes[i] += dt;
		if(game->explodes[i] >= 1 && prev_explode < 1) {
			game->board[i] = 0;
		}

    	float prev_flip = game->flips[i];
    	float prev_fall = game->falls[i];
		game->flips[i]    += dt * 4;
		game->falls[i]    += dt * 6;
		if((game->flips[i] >= 1 && prev_flip < 1) || (game->falls[i] >= 1 && prev_fall < 1)) {
    		update_matches = true;
		}
	}

	// Tick cursor
	game->cursor_anim_t += dt * 12;

	if(update_matches) {
		game_update_matches(game, audio);
	}
}

void game_draw(struct Game* game, struct DrawContext* ctx) {
	uint8_t boardx = LOGICAL_W / 2 - BOARD_W / 2 * 8;
	uint8_t boardy_abs = LOGICAL_H / 2 - BOARD_H / 2 * 8;
	uint8_t boardy = boardy_abs + game->yoff;

	// Skip these tiles during conventional drawing
	bool skips[BOARD_LEN];
	for(uint8_t i = 0; i < BOARD_LEN; i++) {
    	skips[i] = game->board[i] == 0;
	}

	// Draw flips
	for(uint8_t i = 0; i < BOARD_LEN; i++) {
		if(game->flips[i] > 0.5) {
    		continue;
		}
    	
    	// left and right refer to the new tile positions, which might be confusing,
    	// but reflects the current truth.
    	uint8_t flipx;
    	uint8_t flipy;
    	coords_from_index(i, &flipx, &flipy);

    	struct IRect spr;
    	struct Pallete pl;
		float anim_t = game->flips[i] * 2;

		if(game->board[i] != 0) {
        	spr_from_index(game->board, i, &spr, &pl);
        	spr.w *= 2;
        	spr.x += SPR_TILE_MOVE_OFFSET;
        	draw_anim_flip(ctx, anim_t, SPR_TILE_MOVE_FRAMES, spr, boardx + flipx * 8, boardy + flipy * 8, pl, true);
		}

		if(game->board[i + 1] != 0) {
        	spr_from_index(game->board, i + 1, &spr, &pl);
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
    	spr_from_index(game->board, i, &spr, &pl);

		uint8_t x;
		uint8_t y;
		coords_from_index(i, &x, &y);

		// Offset y if currently falling
		uint8_t yoff = 0;
        if(game->falls[i] < 1) {
            yoff = 8 * game->falls[i] - 8;
        }

        if(game->explodes[i] < 1) {
        	pl = PL_ALL_WHITE;
		}
        draw_sprite(ctx, spr, boardx + x * 8, boardy + y * 8 + yoff, pl);
	}

	// Cursor
	{
    	uint8_t curx;
    	uint8_t cury;
    	coords_from_index(game->cursor, &curx, &cury);
    	curx = boardx + curx * 8;
    	cury = boardy + cury * 8;
    	if(game->cursor_anim_t < 1) {
        	uint8_t frames = SPR_CURSOR_MOVE_FRAMES;
    		if(game->cursor_anim_prev == game->cursor + 1) {
    			struct IRect spr = SPR_CURSOR_MOVE_R;
    			draw_anim_flip(ctx, game->cursor_anim_t, frames, spr, curx - 2, cury - 1, PL_ALL_WHITE, true);
    		} else if(game->cursor_anim_prev == game->cursor - 1) {
				struct IRect spr = SPR_CURSOR_MOVE_R;
    			draw_anim(ctx, game->cursor_anim_t, frames, spr, curx - 9, cury - 1, PL_ALL_WHITE);
    		} else if(game->cursor_anim_prev == game->cursor - BOARD_W) {
				struct IRect spr = SPR_CURSOR_MOVE_D;
    			draw_anim(ctx, game->cursor_anim_t, frames, spr, curx - 1, cury - 9, PL_ALL_WHITE);
    		} else if(game->cursor_anim_prev == game->cursor + BOARD_W) {
				struct IRect spr = SPR_CURSOR_MOVE_D;
    			draw_anim_flip(ctx, game->cursor_anim_t, frames, spr, curx - 1, cury - 2, PL_ALL_WHITE, 2);
    		}
    	} else {
        	draw_sprite(ctx, SPR_CURSOR, curx - 1, cury - 1, PL_ALL_WHITE);
    	}
	}

	// Draw border
	draw_fill_rect(ctx, (struct IRect){boardx, boardy_abs + BOARD_H * 8 - 1, BOARD_W * 8, 10}, PL_ALL_BLACK);
	draw_rect(ctx, (struct IRect){boardx - 3, boardy_abs - 3, BOARD_W * 8 + 4, BOARD_H * 8 + 3}, PL_ALL_WHITE);
}

void game_update_matches(struct Game* game, struct AudioContext* audio) {
    bool match_found = false;

	// Horizontal matches
	for(uint8_t y = 0; y < BOARD_H - 1; y++) {
		for(uint8_t x = 1; x < BOARD_W - 1; x++) {
			uint8_t check_tile = game->board[y * BOARD_W + x - 1];
			if(check_tile == 0) {
				continue;
			}
			
			uint8_t match_count = 1;
			while(x < BOARD_W && game->board[y * BOARD_W + x] == check_tile) {
				match_count++;
				x++;
			}
			if(match_count >= 3) {
    			match_found = true;
				for(uint8_t matchx = x; matchx > x - match_count; matchx--) {
					game->explodes[y * BOARD_W + matchx - 1] = 0;
				}
			}
		}
	}
	// Vertical matches
	for(uint8_t x = 0; x < BOARD_W; x++) {
		for(uint8_t y = 1; y < BOARD_H - 2; y++) {
			uint8_t check_tile = game->board[(y - 1) * BOARD_W + x];
			if(check_tile == 0) {
				continue;
			}
			
			uint8_t match_count = 1;
			while(y < BOARD_H - 1 && game->board[y * BOARD_W + x] == check_tile) {
				match_count++;
				y++;
			}
			if(match_count >= 3) {
    			match_found = true;
				for(uint8_t matchy = y; matchy > y - match_count; matchy--) {
					game->explodes[(matchy - 1) * BOARD_W + x] = 0;
				}
			}
		}
	}

	if(match_found) {
		game->hitch = 0;
		game->hitch = 0;

		struct Sound sound;
    	sound.priority = 1;
    	sound.callback = snd_match;
    	sound_play(audio, sound);
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
