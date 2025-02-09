#include "game.h"

#include "stdbool.h"
#include "stdio.h"

#include "sounds.h"
#include "board.h"
#include "matches.h"

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
    		if(falling(game, above) 
    		|| exploding(game, above)
    		|| flipping(game, i) || flipping(game, above) 
    		|| !empty(game, i) || empty(game, above)) {
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
