#include "game.h"

#include "stdbool.h"
#include "stdio.h"
#include "sounds.h"

// TODO: Fixes/improvements
// - How to signal that the bottom row is unselectable? All grey? Inverse? Kinda
//   still needs to be clear what type of tile the thing is.

void game_init(struct Game* game) {
	game->yoff = 0;
	game->yoff_countdown = FRAMES_YOFF;
	game->timer = 0;

	game->cursor = BOARD_LEN / 2;
	game->cursor_prev = game->cursor;
	game->cursor_anim = FRAMES_CURSOR;

	for(uint8_t i = 0; i < BOARD_LEN; i++) {
		game->tiles[i] = rand() % (SHAPES_LEN + 1);
		if(rand() % 2 == 0) {
    		game->tiles[i] = 0;
		}

		game->flips[i] = 0;
		game->explodes[i] = 0;
		game->falls[i] = 0;
		game->buf_falls[i] = 0;
	}

	for(uint8_t x = 0; x < BOARD_W; x++) {
		game->tiles[x] = 0;
	}

	game->state = GAME_PRE;
}

void game_control(struct Game* game, struct Input* input, struct AudioContext* audio) {
    // Updated and used throughout function
	uint8_t curx = xcoord(game->cursor);
	uint8_t cury = ycoord(game->cursor);

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

    	uint8_t cursor_new = bindex(curx, cury);
		// Keep track of the previous cursor position for animation
    	if(cursor_new != game->cursor) {
    		game->cursor_prev = game->cursor;
    		game->cursor_anim = 0;
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
    	if(!can_flip(game)) {
        	sound.callback = snd_noflip;
        } else {
        	sound.callback = snd_flip;

			uint8_t right = xoffset(game->cursor, 1);

        	uint8_t tmp = game->tiles[game->cursor];
        	game->tiles[game->cursor] = game->tiles[right];
        	game->tiles[right] = tmp;

        	// Cancel adjacent flip animations
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
        	check_matches(game, audio);
    	}
    	sound_play(audio, sound);
	}
}

void game_tick(struct Game* game, struct AudioContext* audio) {
	bool update_matches = false;

	if(game->timer != 0) {
    	game->timer--;
		goto postmove;
	}
	
	// Move tiles up, generating new tiles as needed
	game->yoff_countdown -= 1;
	if(game->yoff_countdown == 0) {
    	game->yoff += 1;
    	game->yoff_countdown = FRAMES_YOFF;
	}

	if(game->yoff != 8) {
		goto postmove;
	}

	// Only do this bit if we are resetting yoff

	// End game if we have tiles at the top
	for(uint8_t x = 0; x < BOARD_W; x++) {
		if(game->tiles[bindex(x, 0)] != 0) {
			game->state = GAME_POST;

    		struct Sound sound;
    		sound.priority = 1;
    		sound.callback = snd_lose;
    		sound_play(audio, sound);
			return;
		}
	}

	game->yoff = 0;

	// Move everything up one tile, including events
	if(game->cursor >= BOARD_W) {
		game->cursor -= BOARD_W;
	}
	for(uint8_t y = 1; y < BOARD_H; y++) {
		for(uint8_t x = 0; x < BOARD_W; x++) {
			uint8_t oldpos = bindex(x, y);
			uint8_t newpos = yoffset(oldpos, -1);

			game->tiles[newpos]     = game->tiles[oldpos];
			game->flips[newpos]     = game->flips[oldpos];
			game->explodes[newpos]  = game->explodes[oldpos];
			game->falls[newpos]     = game->falls[oldpos];
			game->buf_falls[newpos] = game->buf_falls[oldpos];
		}
	}

	// Generate new tiles
	for(uint8_t x = 0; x < BOARD_W; x++) {
		game->tiles[bindex(x, BOARD_H - 1)] = 1 + rand() % (SHAPES_LEN);
	}

	// Now that we have a new row of tiles, we need to check for matches.
	update_matches = true;

// Skipped to if we haven't reset yoff
postmove:

	// Update event ticks
	for(uint8_t i = 0; i < BOARD_LEN; i++) {
    	if(game->buf_falls[i] != 0) {
    		game->buf_falls[i]--;
    	}

    	// Constructed this way, the following if statements will only deincrement
    	// the associate values if they don't already equal 0.
    	if(game->explodes[i] != 0 && --game->explodes[i] == 0) {
			game->tiles[i] = 0;
    	}

    	if(game->flips[i] != 0 && --game->flips[i] == 0) {
    		update_matches = true;
    	}

    	if(game->falls[i] != 0 && --game->falls[i] == 0) {
    		update_matches = true;
    	}
	}

	// Check for tile falling
	for(uint8_t x = 0; x < BOARD_W; x++) {
    	for(uint8_t y = BOARD_H - 1; y >= 1; y--) {
        	uint8_t i = bindex(x, y);
        	uint8_t above = yoffset(i, -1);

        	// Skip anything which isn't a non-empty tile on top of an empty tile,
        	// also excluding falls which are already in motion on the upper tile.
    		if(falling(game, above) || flipping(game, above) || flipping(game, i) || exploding(game, above)
    		|| game->tiles[i] != 0 || game->tiles[above] == 0) {
				continue;
    		}

			// Move the tile down
			game->tiles[i] = game->tiles[above];
			game->tiles[above] = 0;

			// Keep track of the fall and cancel any associated buffered falls
			game->falls[i] = FRAMES_FALL;
			game->buf_falls[above] = 0;
    	}
	}

	// Tick cursor
	if(game->cursor_anim != FRAMES_CURSOR) {
    	game->cursor_anim++;
	}

	if(update_matches) {
		check_matches(game, audio);
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
	uint8_t boardy = boardy_abs - game->yoff + 8;

	// Skip empty tiles during tile draw below
	bool skips[BOARD_LEN];
	for(uint8_t i = 0; i < BOARD_LEN; i++) {
    	skips[i] = game->tiles[i] == 0;
	}

	// Draw flips
	for(uint8_t i = 0; i < BOARD_LEN; i++) {
		if(game->flips[i] < (FRAMES_FLIP / 2) + 1) {
    		continue;
		}
    	
    	uint8_t flipx = xcoord(i);
    	uint8_t flipy = ycoord(i);

    	struct IRect spr;
    	struct Pallete pl;
		float anim_t = FRAMES_FLIP - game->flips[i];

		if(game->tiles[i] != 0) {
        	spr_from_index(game->tiles, i, &spr, &pl);
        	spr.w *= 2;
        	spr.x += SPR_TILE_MOVE_OFFSET;
        	draw_anim_flip(ctx, anim_t, SPR_TILE_MOVE_FRAME_LEN, spr, boardx + flipx * 8, boardy + flipy * 8, pl, true);
		}

		// We assume i + 1 is safe, because flips never appear at a right edge tile
		if(game->tiles[i + 1] != 0) {
        	spr_from_index(game->tiles, i + 1, &spr, &pl);
        	spr.w *= 2;
        	spr.x += SPR_TILE_MOVE_OFFSET;
        	draw_anim(ctx, anim_t, SPR_TILE_MOVE_FRAME_LEN, spr, boardx + flipx * 8, boardy + flipy * 8, pl);
		}

		// Skip flipping tiles during tile draw below
    	skips[i] = true;
    	// See above on safety of i + 1
    	skips[xoffset(i, 1)] = true;
	}

	// Tile draw - skipping skips
    for(uint8_t i = 0; i < BOARD_LEN; i++) {
		if(skips[i]) {
    		continue;
		}
        
    	struct IRect spr;
    	struct Pallete pl;
    	spr_from_index(game->tiles, i, &spr, &pl);

		uint8_t x = xcoord(i);
		uint8_t y = ycoord(i);

		// Offset y if currently falling
		uint8_t yoff = 0;
        if(game->falls[i] != 0) {
			yoff = (game->falls[i] * 8) / FRAMES_FALL;
        }

        if(game->explodes[i] != 0 && (game->explodes[i] / 12) % 2 != 0) {
        	pl = PL_ALL_WHITE;
		}
		if(y == BOARD_H - 1) {
    		pl = PL_ALL_WHITE;
		}
		
        draw_sprite(ctx, spr, boardx + x * 8, boardy + y * 8 - yoff, pl);
	}

	// Cursor
	{
    	uint8_t curx = xcoord(game->cursor) * 8 + boardx;
    	uint8_t cury = ycoord(game->cursor) * 8 + boardy;

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

	// Draw debug gizmos
	for(int i = 0; i < BOARD_LEN; i++) {
		if(flipping(game, i)) draw_sprite(ctx, SPR_DEBUG_1, boardx + i % BOARD_W * 8, boardy + i / BOARD_W * 8, PL_BLUE);
		//if(exploding(game, i)) draw_sprite(ctx, SPR_DEBUG_2, boardx + i % BOARD_W * 8, boardy + i / BOARD_W * 8, PL_BLUE);
		//if(falling(game, i)) draw_sprite(ctx, SPR_DEBUG_1, boardx + i % BOARD_W * 8, boardy + i / BOARD_W * 8, PL_PURPLE);
		//if(fall_buffered(game, i)) draw_sprite(ctx, SPR_DEBUG_2, boardx + i % BOARD_W * 8, boardy + i / BOARD_W * 8, PL_YELLOW);
	}
}

void game_draw_pre(struct Game* game, struct DrawContext* ctx) {
	(void)game;
	(void)ctx;
}

void game_draw_post(struct Game* game, struct DrawContext* ctx) {
	uint8_t boardx = LOGICAL_W / 2 - BOARD_W / 2 * 8;
	uint8_t boardy_abs = LOGICAL_H / 2 - BOARD_H / 2 * 8;
	uint8_t boardy = boardy_abs - game->yoff + 8;

    for(uint8_t i = 0; i < BOARD_LEN; i++) {  
		if(game->tiles[i] == 0) {
    		continue;
		}

    	struct IRect spr;
    	struct Pallete pl;
    	spr_from_index(game->tiles, i, &spr, &pl);

		uint8_t x = xcoord(i);
		uint8_t y = ycoord(i);

		// Offset y if currently falling
		uint8_t yoff = 0;
        if(game->falls[i] != 0) {
			yoff = (game->falls[i] * 8) / FRAMES_FALL;
        }

        if((game->timer / 16) % 2 != 0) {
        	pl = PL_ALL_WHITE;
		}
        draw_sprite(ctx, spr, boardx + x * 8, boardy + y * 8 - yoff, pl);
	}
}

void check_matches(struct Game* game, struct AudioContext* audio) {
    uint8_t new[BOARD_LEN];;
    uint8_t new_len = 0;

	// Rows
	for(uint8_t y = 0; y < BOARD_H - 1; y++) {
		for(uint8_t x = 0; x < BOARD_W - 1; x++) {
    		uint8_t i = bindex(x, y);

			uint8_t tile = game->tiles[i];
			if(tile == 0) {
				continue;
			}
			
			uint8_t matches = 0;
			uint8_t xoff = 0;
			while(x + xoff < BOARD_W) {
        		uint8_t check = xoffset(i, xoff);
    			if(game->tiles[check] != tile || exploding(game, check) || falling(game, check) || fall_buffered(game, check)) {
    				break;
    			}
    			matches++;
    			xoff++;
    		}

    		if(matches >= 3) {
    			for(uint8_t j = 0; j < matches; j++) {
        			new[new_len] = xoffset(i, j);
        			new_len++;
    			}
    		}

    		if(xoff == 0) {
        		xoff = 1;
    		}
    		x += xoff - 1;
		}
	}
	// Columns
	for(uint8_t x = 0; x < BOARD_W; x++) {
		for(uint8_t y = 0; y < BOARD_H - 2; y++) {
    		uint8_t i = bindex(x, y);

			uint8_t tile = game->tiles[i];
			if(tile == 0) {
				continue;
			}
			
			uint8_t matches = 0;
			uint8_t yoff = 0;
			while(y + yoff < BOARD_H - 1) {
        		uint8_t check = yoffset(i, yoff);
    			if(game->tiles[check] != tile || exploding(game, check) || falling(game, check) || fall_buffered(game, check)) {
    				break;
    			}
    			matches++;
    			yoff++;
    		}
    		if(matches >= 3) {
    			for(uint8_t j = 0; j < matches; j++) {
        			new[new_len] = yoffset(i, j);
        			new_len++;
    			}
    		}
    		if(yoff == 0) {
        		yoff = 1;
    		}
    		y += yoff - 1;
		}
	}

	for(int i = 0; i < new_len; i++) {
    	game->explodes[new[i]] = FRAMES_EXPLODE;
	}

	if(new_len > 0) {
		game->timer = FRAMES_HITCH;

		struct Sound sound;
    	sound.priority = 1;
    	sound.callback = snd_match;
    	sound_play(audio, sound);
	}
}

bool empty(struct Game* game, uint8_t i) {
	return game->tiles[i] == 0;
}

bool flipping(struct Game* game, uint8_t i) {
    bool flipping = game->flips[i] != 0;
    return flipping || (i % BOARD_W != BOARD_W - 1 && game->flips[i - 1] != 0);
}

bool exploding(struct Game* game, uint8_t i) {
	return game->explodes[i] != 0;
}

bool falling(struct Game* game, uint8_t i) {
    bool falling = game->falls[i] != 0;
    return falling || (i / BOARD_W != 0 && game->falls[i - BOARD_W] != 0);
}

bool fall_buffered(struct Game* game, uint8_t i) {
    return game->buf_falls[i] != 0;
}

uint8_t xcoord(uint8_t i) {
	return i % BOARD_W;
}

uint8_t ycoord(uint8_t i) {
	return i / BOARD_W;
}

uint8_t bindex(uint8_t x, uint8_t y) {
    return y * BOARD_W + x;
}

uint8_t xoffset(uint8_t i, uint8_t x) { 
	return i + x;
}

uint8_t yoffset(uint8_t i, uint8_t y) {
	return i + BOARD_W * y;
}

uint8_t offset(uint8_t i, uint8_t x, uint8_t y) {
	 return i + x + BOARD_W * y;
}

bool can_flip(struct Game* game) {
	// Don't allow flip if both cursor tiles empty, or either one is exploding or
	// part of a fall.
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

	return true;
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
