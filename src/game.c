#include "game.h"

#include "stdbool.h"
#include "stdio.h"

#include "sounds.h"
#include "board.h"
#include "matches.h"

// Initializes a game round. Called every time a new round is started.
void game_init(struct Game* game) {
	game->speed = 225;
	game->speed_blink = 0;
	game->speed_countdown = FRAMES_SPEED_COUNTDOWN;

	game->yoff = 0;
	game->yoff_countdown = -game->speed;
	game->grace_period = false;

	game->score = 0;
	game->score_visible = game->score;
	game->score_countup = 0;
	game->score_blink = 0;

	game->cursor_init(game);
	for(int i = 0; i < MAX_CURSORS; i++) {
		game->cursors[i].prev = game->cursors[i].pos;
		game->cursors[i].anim = FRAMES_CURSOR;
	}

	// Generate some starting tiles and initialize events to 0.
	for(uint8_t i = 0; i < BOARD_LEN; i++) {
		game->tiles[i] = rand() % (SHAPES_LEN + 1);
		if(rand() % 2 == 0) {
			game->tiles[i] = 0;
		}

		game->matches[i]   = 0;
		game->falls[i]	   = 0;
		game->buf_falls[i] = 0;
		game->shifts[i].t  = 0;
	}

	// Clear top row of tiles
	for(uint8_t x = 0; x < BOARD_W; x++) {
		game->tiles[x] = 0;
	}

	game->state = GAME_PRE;
}

// Modifies the game state based on player input. Immediately precedes game_tick().
void game_control(struct Game* game, struct Input* input, struct AudioContext* audio) {
	if(game->move_cursor(game, input)) {
		sound_play_new(audio, snd_move, 1, NULL);
	}

	// Shift tiles
	if(input->select.just_pressed) {
		if(game->shift(game)) {
			check_matches(game, audio);
			sound_play_new(audio, snd_shift, 1, NULL);
		} else {
			sound_play_new(audio, snd_noshift, 1, NULL);
		}
	}
}

// Simulates a single tick of game logic. 
void game_tick(struct Game* game, struct AudioContext* audio) {
	bool should_match = false;

	game->yoff_countdown--;
	if(game->yoff_countdown != 0) {
		goto post_yoff_logic;
	}

	if(!game->grace_period) {
		goto iterate_yoff;
	}

	// Grace period logic
	if(tiles_reached_top(game)) {
		game->state = GAME_POST;
		sound_play_new(audio, snd_lose, 1, NULL);
		return;
	} 		

	// Player managed to clear up top of the board before the grace period expired.
	game->grace_period = false;
	goto generate_new_row;

iterate_yoff:
	game->yoff_countdown = -game->speed;

	game->yoff += 1;
	if(game->yoff != 8) {
		goto post_yoff_logic;
	}

	if(tiles_reached_top(game)) {
		game->yoff_countdown = FRAMES_GRACE;
		game->grace_period = true;
		goto post_yoff_logic;
	}
	goto generate_new_row;

// Move everything up one tile, including events
generate_new_row:
	for(int i = 0; i < MAX_CURSORS; i++) {
		if(game->cursors[i].pos >= BOARD_W) {
			game->cursors[i].pos -= BOARD_W;
		}
	}
	for(uint8_t y = 1; y < BOARD_H; y++) {
		for(uint8_t x = 0; x < BOARD_W; x++) {
			uint8_t oldpos = bindex(x, y);
			uint8_t newpos = yoffset(oldpos, -1);

			game->tiles[newpos]	    = game->tiles[oldpos];
			game->matches[newpos]   = game->matches[oldpos];
			game->falls[newpos]	    = game->falls[oldpos];
			game->buf_falls[newpos] = game->buf_falls[oldpos];

			game->shifts[newpos] = game->shifts[oldpos];
			game->shifts[newpos].to_pos = yoffset(game->shifts[newpos].to_pos, -1);
		}
	}

	// Generate new tiles
	for(uint8_t x = 0; x < BOARD_W; x++) {
		game->tiles[bindex(x, BOARD_H - 1)] = 1 + rand() % (SHAPES_LEN);
	}

	// Now that we have a new row of tiles, we need to check for matches.
	should_match = true;
	game->yoff = 0;

// Thus marks the end of the gotos. They are, after all, considered harmful.
post_yoff_logic:

	// Increase visible score to the actual score
	if(game->score_visible < game->score && game->score_countup != 0) {
		game->score_countup--;
		if(game->score_countup == 0) {
			game->score_visible++;
			if(game->score_visible < game->score) {
				game->score_countup = FRAMES_SCORE_COUNTUP;
			}
		}
	}

	// Update blinking text
	if(game->score_blink != 0) {
		game->score_blink--;
	}

	if(game->speed_blink != 0) {
		game->speed_blink--;
	}

	// Countdown to speed increase
	game->speed_countdown--;
	if(game->speed_countdown == 0) {
		game->speed++;
		game->speed_countdown = FRAMES_SPEED_COUNTDOWN;
		game->speed_blink = FRAMES_SPEED_BLINK;
	}

	// Update event ticks (buf falls, matches, falls, and shifts)
	for(uint8_t i = 0; i < BOARD_LEN; i++) {
		if(game->buf_falls[i] != 0) {
			game->buf_falls[i]--;
		}
		// Constructed this way, the following if statements will only deincrement
		// the associate values if they don't already equal 0.
		if(game->matches[i] != 0 && --game->matches[i] == 0) {
			game->tiles[i] = 0;
		}
		if(game->falls[i] != 0 && --game->falls[i] == 0) {
			should_match = true;
		}

		if(game->shifts[i].t != 0 && --game->shifts[i].t == 0) {
			should_match = true;
		}
	}

	// Check for tile falling
	for(uint8_t x = 0; x < BOARD_W; x++) {
		for(uint8_t y = BOARD_H - 1; y >= 1; y--) {
			uint8_t i = bindex(x, y);
			uint8_t above = yoffset(i, -1);

			// Skip anything which isn't a non-empty tile on top of an empty tile,
			// also excluding falls which are already in motion on the upper tile.
			if(falling(game, above) || matching(game, above) || shifting(game, i) || shifting(game, above) || !empty(game, i) || empty(game, above)) {
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
	for(int i = 0; i < MAX_CURSORS; i++) {
		if(game->cursors[i].anim != FRAMES_CURSOR) {
			game->cursors[i].anim++;
		}
	}

	if(should_match) {
		check_matches(game, audio);
	}
}

bool tiles_reached_top(struct Game* game) {
	for(uint8_t x = 0; x < BOARD_W; x++) {
		if(game->tiles[bindex(x, 0)] != 0) {
			return true;
		}
	}
	return false;
}
