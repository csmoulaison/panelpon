#include "game.h"

#include "stdbool.h"
#include "stdio.h"
#include "sounds.h"

// TODO: Fixes/improvements
// - Implement priority system to sounds so we can at least see what a less
//   chaotic soundscape would be like. Kinda cool as is, though. Kinda fucked.
// - How to signal that the bottom row is unselectable? All grey? Inverse? Kinda
//   still needs to be clear what type of tile the thing is.

void game_init(struct Game* game) {
	game->yoff = 0;
	game->hitch = 1;

	game->cursor = 0;
	game->cursor_prev = game->cursor;
	game->cursor_anim_t = 1;

	for(uint8_t i = 0; i < BOARD_LEN; i++) {
		game->tiles[i] = rand() % (SHAPES_LEN + 1);
		if(rand() % 2 == 0) game->tiles[i] = 0;
		game->flips[i] = 2;
		game->explodes[i] = 2;
		game->falls[i] = 2;
		game->buf_falls[i] = 2;
	}

	game->state = GAME_PRE;
}

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
    		game->cursor_prev = game->cursor;
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

		// Don't allow flip if both cursor tiles empty, or either one is exploding or
		// part of a fall.
    	if((empty(game, game->cursor) && empty(game, game->cursor + 1)) 
    	|| game->buf_falls[game->cursor] < 1 || game->buf_falls[game->cursor + 1] < 1
    	|| exploding(game, game->cursor) || exploding(game, game->cursor + 1) || falling(game, game->cursor)) {
    		sound.callback = snd_noflip;
    		sound_play(audio, sound);
        	goto endflip;
    	}

    	// Don't allow flip if a cursor tile is empty with a tile somewhere above it.
    	for(int i = 0; i < 2; i++) {
        	uint8_t check_cursor = game->cursor + i;
        	if(!empty(game, check_cursor)) {
            	continue;
        	}
			for(int j = 1; j < BOARD_H; j++) {
				if(check_cursor / BOARD_W - j < 0) {
    				break;
				}
				if(!empty(game, check_cursor - j * BOARD_W)) {
					goto endflip;
				}
			}
    	}

		sound.callback = snd_flip;
    	sound_play(audio, sound);

		uint8_t tmp = game->tiles[game->cursor];
		game->tiles[game->cursor] = game->tiles[game->cursor + 1];
		game->tiles[game->cursor + 1] = tmp;

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

		for(int i = 0; i < 2; i++) {
    		if(empty(game, game->cursor + i + BOARD_W)) {
				game->buf_falls[game->cursor + i] = 0;
    		}
		}
	}

endflip:
}

void game_tick(struct Game* game, struct AudioContext* audio, double dt) {
	bool update_matches = false;

	// Move tiles up, generating new tiles as needed
	game->hitch += dt;
	if(game->hitch < 1) {
		goto postmove;
	}
	
	game->yoff -= dt * 4;
	if(game->yoff < 0) {
		for(uint8_t x = 0; x < BOARD_W; x++) {
			if(game->tiles[0 * BOARD_W + x] != 0) {
				game->state = GAME_POST;
				return;
			}
		}
    	
		game->yoff += 8;

		// Move everything up one tile, including events
		if(game->cursor >= BOARD_W) {
			game->cursor -= BOARD_W;
		}
		for(uint8_t y = 1; y < BOARD_H; y++) {
			for(uint8_t x = 0; x < BOARD_W; x++) {
    			uint8_t oldpos = y * BOARD_W + x;
    			uint8_t newpos = oldpos - BOARD_W;

				game->tiles[newpos]     = game->tiles[oldpos];
				game->flips[newpos]     = game->flips[oldpos];
				game->explodes[newpos]  = game->explodes[oldpos];
				game->falls[newpos]     = game->falls[oldpos];
				game->buf_falls[newpos] = game->buf_falls[oldpos];
			}
		}
		for(uint8_t x = 0; x < BOARD_W; x++) {
    		game->tiles[(BOARD_H - 1) * BOARD_W + x] = 1 + rand() % (SHAPES_LEN);
		}

		update_matches = true;
	}

postmove:

	// Update event ticks
	for(uint8_t i = 0; i < BOARD_LEN; i++) {
    	float prev_explode = game->explodes[i];
		game->explodes[i] += dt;
		if(game->explodes[i] >= 1 && prev_explode < 1) {
			game->tiles[i] = 0;
		}

		float flip_speed = 6;
    	float prev_flip = game->flips[i];
		game->flips[i] += dt * flip_speed;
		game->buf_falls[i] += dt * flip_speed;

    	float prev_fall = game->falls[i];
		game->falls[i] += dt * 10;

		if((game->flips[i] >= 1 && prev_flip < 1) || (game->falls[i] >= 1 && prev_fall < 1)) {
    		update_matches = true;
		}
	}

	// Check for tile falling
	for(uint8_t x = 0; x < BOARD_W; x++) {
    	for(uint8_t y = BOARD_H - 1; y >= 1; y--) {
        	uint8_t i = index_from_coords(x, y);

        	// Skip anything which isn't a non-empty tile on top of an empty tile,
        	// also excluding falls which are already in motion on the upper tile.
    		if(falling(game, i - BOARD_W) || flipping(game, i - BOARD_W) || flipping(game, i) || exploding(game, i - BOARD_W)
    		|| game->tiles[i] != 0 || game->tiles[i - BOARD_W] == 0) {
				continue;
    		}

			game->tiles[i] = game->tiles[i - BOARD_W];
			game->tiles[i - BOARD_W] = 0;
			game->falls[i] = 0;
			game->buf_falls[i - BOARD_W] = 2;
    	}
	}

	// Tick cursor
	game->cursor_anim_t += (float)dt * 12;

	if(update_matches) {
		game_update_matches(game, audio);
	}
}

void game_draw(struct Game* game, struct DrawContext* ctx) {
	uint8_t boardx = LOGICAL_W / 2 - BOARD_W / 2 * 8;
	uint8_t boardy_abs = LOGICAL_H / 2 - BOARD_H / 2 * 8;
	uint8_t boardy = boardy_abs + game->yoff;
	(void)boardy;

	switch(game->state) {
    	case GAME_ACTIVE:
        	game_draw_active(game, ctx);
        	break;
    	case GAME_PRE:
        	game_draw_pre(game, ctx);
        	break;
    	case GAME_POST:
        	game_draw_post(game, ctx);
        	break;
        default:
            break;
	}

	// Draw border
	draw_fill_rect(ctx, (struct IRect){boardx, boardy_abs + BOARD_H * 8 - 1, BOARD_W * 8, 10}, PL_ALL_BLACK);
	draw_rect(ctx, (struct IRect){boardx - 3, boardy_abs - 3, BOARD_W * 8 + 4, BOARD_H * 8 + 3}, PL_ALL_WHITE);
}

void game_draw_active(struct Game* game, struct DrawContext* ctx) {
	uint8_t boardx = LOGICAL_W / 2 - BOARD_W / 2 * 8;
	uint8_t boardy_abs = LOGICAL_H / 2 - BOARD_H / 2 * 8;
	uint8_t boardy = boardy_abs + game->yoff;

	// Skip empty tiles during tile draw below
	bool skips[BOARD_LEN];
	for(uint8_t i = 0; i < BOARD_LEN; i++) {
    	skips[i] = game->tiles[i] == 0;
	}

	// Draw flips
	for(uint8_t i = 0; i < BOARD_LEN; i++) {
		if(game->flips[i] > 0.75) {
    		continue;
		}
    	
    	uint8_t flipx;
    	uint8_t flipy;
    	coords_from_index(i, &flipx, &flipy);

    	struct IRect spr;
    	struct Pallete pl;
		float anim_t = game->flips[i] * 2;

		if(game->tiles[i] != 0) {
        	spr_from_index(game->tiles, i, &spr, &pl);
        	spr.w *= 2;
        	spr.x += SPR_TILE_MOVE_OFFSET;
        	draw_anim_flip(ctx, anim_t, SPR_TILE_MOVE_FRAMES, spr, boardx + flipx * 8, boardy + flipy * 8, pl, true);
		}

		// We assume i + 1 is safe, because flips never appear at a right edge tile
		if(game->tiles[i + 1] != 0) {
        	spr_from_index(game->tiles, i + 1, &spr, &pl);
        	spr.w *= 2;
        	spr.x += SPR_TILE_MOVE_OFFSET;
        	draw_anim(ctx, anim_t, SPR_TILE_MOVE_FRAMES, spr, boardx + flipx * 8, boardy + flipy * 8, pl);
		}

		// Skip flipping tiles during tile draw below
    	skips[i] = true;
    	// See above on safety of i + 1
    	skips[i + 1] = true;
	}

	// Tile draw - skipping skips
    for(uint8_t i = 0; i < BOARD_LEN; i++) {
		if(skips[i]) {
    		continue;
		}
        
    	struct IRect spr;
    	struct Pallete pl;
    	spr_from_index(game->tiles, i, &spr, &pl);

		uint8_t x;
		uint8_t y;
		coords_from_index(i, &x, &y);

		// Offset y if currently falling
		uint8_t yoff = 0;
        if(game->falls[i] < 1) {
            yoff = 8 * game->falls[i] - 8;
        }

        if(game->explodes[i] < 1 && (int)(game->explodes[i] * 16) % 2 != 0) {
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
    		if(game->cursor_prev == game->cursor + 1) {
    			struct IRect spr = SPR_CURSOR_MOVE_R;
    			draw_anim_flip(ctx, game->cursor_anim_t, frames, spr, curx - 2, cury - 1, PL_ALL_WHITE, true);
    		} else if(game->cursor_prev == game->cursor - 1) {
				struct IRect spr = SPR_CURSOR_MOVE_R;
    			draw_anim(ctx, game->cursor_anim_t, frames, spr, curx - 9, cury - 1, PL_ALL_WHITE);
    		} else if(game->cursor_prev == game->cursor - BOARD_W) {
				struct IRect spr = SPR_CURSOR_MOVE_D;
    			draw_anim(ctx, game->cursor_anim_t, frames, spr, curx - 1, cury - 9, PL_ALL_WHITE);
    		} else if(game->cursor_prev == game->cursor + BOARD_W) {
				struct IRect spr = SPR_CURSOR_MOVE_D;
    			draw_anim_flip(ctx, game->cursor_anim_t, frames, spr, curx - 1, cury - 2, PL_ALL_WHITE, 2);
    		}
    	} else {
        	draw_sprite(ctx, SPR_CURSOR, curx - 1, cury - 1, PL_ALL_WHITE);
    	}
	}

	// Draw debug gizmos
	for(int i = 0; i < BOARD_LEN; i++) {
		//if(flipping(game, i)) draw_sprite(ctx, SPR_DEBUG_1, boardx + i % BOARD_W * 8, boardy + i / BOARD_W * 8, PL_BLUE);
		//if(exploding(game, i)) draw_sprite(ctx, SPR_DEBUG_2, boardx + i % BOARD_W * 8, boardy + i / BOARD_W * 8, PL_BLUE);
		//if(falling(game, i)) draw_sprite(ctx, SPR_DEBUG_1, boardx + i % BOARD_W * 8, boardy + i / BOARD_W * 8, PL_BLUE);
		if(game->buf_falls[i] < 1) draw_sprite(ctx, SPR_DEBUG_1, boardx + i % BOARD_W * 8, boardy + i / BOARD_W * 8, PL_BLUE);
	}
}

void game_draw_pre(struct Game* game, struct DrawContext* ctx) {
	(void)game;
	(void)ctx;
}

void game_draw_post(struct Game* game, struct DrawContext* ctx) {
	uint8_t boardx = LOGICAL_W / 2 - BOARD_W / 2 * 8;
	uint8_t boardy_abs = LOGICAL_H / 2 - BOARD_H / 2 * 8;
	uint8_t boardy = boardy_abs + game->yoff;

    for(uint8_t i = 0; i < BOARD_LEN; i++) {  
		if(game->tiles[i] == 0) {
    		continue;
		}
        
    	struct IRect spr;
    	struct Pallete pl;
    	spr_from_index(game->tiles, i, &spr, &pl);

		uint8_t x;
		uint8_t y;
		coords_from_index(i, &x, &y);

		// Offset y if currently falling
		uint8_t yoff = 0;
        if(game->falls[i] < 1) {
            yoff = 8 * game->falls[i] - 8;
        }

        if((int)(game->hitch * 2) % 2 != 0) {
        	pl = PL_ALL_WHITE;
		}
        draw_sprite(ctx, spr, boardx + x * 8, boardy + y * 8 + yoff, pl);
	}
}

void game_update_matches(struct Game* game, struct AudioContext* audio) {
    bool matches_found = false;
	// Horizontal matches
	for(uint8_t y = 0; y < BOARD_H - 1; y++) {
		for(uint8_t x = 0; x < BOARD_W - 1; x++) {
			uint8_t tile = game->tiles[y * BOARD_W + x];
			if(tile == 0) {
				continue;
			}
			
			uint8_t matches = 0;
			uint8_t off = 0;
			while(x + off < BOARD_W) {
        		uint8_t check = y * BOARD_W + x + off;
    			if(game->tiles[check] != tile || exploding(game, check) || falling(game, check) || game->buf_falls[check] < 1) {
    				break;
    			}
    			matches++;
    			off++;
    		}
    		if(matches >= 3) {
    			for(uint8_t i = 0; i < matches; i++) {
    				game->explodes[y * BOARD_W + x + i] = 0;
    			}
    			matches_found = true;
    		}
    		if(off == 0) off++;
    		x += off - 1;
		}
	}
	for(uint8_t x = 0; x < BOARD_W; x++) {
		for(uint8_t y = 0; y < BOARD_H - 2; y++) {
			uint8_t tile = game->tiles[y * BOARD_W + x];
			if(tile == 0) {
				continue;
			}
			
			uint8_t matches = 0;
			uint8_t off = 0;
			while(y + off < BOARD_H - 1) {
        		uint8_t check = (y + off) * BOARD_W + x;
    			if(game->tiles[check] != tile || exploding(game, check) || falling(game, check) || game->buf_falls[check] < 1) {
    				break;
    			}
    			matches++;
    			off++;
    		}
    		if(matches >= 3) {
    			for(uint8_t i = 0; i < matches; i++) {
    				game->explodes[(y + i) * BOARD_W + x] = 0;
    			}
    			matches_found = true;
    		}
    		if(off == 0) off++;
    		y += off - 1;
		}
	}

	if(matches_found) {
		game->hitch = 0;

		struct Sound sound;
    	sound.priority = 1;
    	sound.callback = snd_match;
    	sound_play(audio, sound);
	}
}

bool empty(struct Game* game, uint8_t i) {
	return game->tiles[i] < 1;
}

bool flipping(struct Game* game, uint8_t i) {
    bool flipping = game->flips[i] < 1;
    return flipping || (i % BOARD_W != BOARD_W - 1 && game->flips[i - 1] < 1);
}

bool exploding(struct Game* game, uint8_t i) {
	return game->explodes[i] < 1;
}

bool falling(struct Game* game, uint8_t i) {
    bool falling = game->falls[i] < 1;
    return falling || (i / BOARD_W != 0 && game->falls[i - BOARD_W] < 1);
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
        case 5:
            *spr = SPR_TRIANGLE;
            *pl = PL_YELLOW;
            break;
        case 6:
            *spr = SPR_CAPSULE;
            *pl = PL_CYAN;
            break;
        default:
            break;
	}
}
