#include "game.h"

#include "stdbool.h"
#include "stdio.h"

#include "sounds.h"
#include "board.h"
#include "matches.h"
#include "context.h"
#include "cursor_callbacks.h"
#include "speed_lut.h"
#include "transition.h"

void game_enter(struct Context* ctx) {
    ctx->prog_state = PROG_GAME;
    game_init(&ctx->game);
}

// Initializes a game round. Called every time a new round is started.
void game_init(struct Game* game) {
	game->speed = game->start_speed;
	game->speed_blink = 0;
	game->speed_countdown = game->speed_increase_interval * 1250;

	game->yoff = 0;
	game->yoff_countdown = speeds[game->speed];
	game->grace_period = false;

	game->score = 0;
	game->score_visible = game->score;
	game->score_countup = 0;
	game->score_blink = 0;

	switch(game->cursor_type) {
		case CUR_CLASSIC:
			game->cursor_init = cur_classic_init;
			game->shift = cur_classic_shift;
			game->draw_cursor = cur_classic_draw;
			game->move_cursor = cur_classic_move;
			break;
		case CUR_VERT:
			game->cursor_init = cur_vert_init;
			game->shift = cur_vert_shift;
			game->draw_cursor = cur_vert_draw;
			game->move_cursor = cur_vert_move;
			break;
		case CUR_WARP:
			game->cursor_init = cur_warp_init;
			game->shift = cur_warp_shift;
			game->draw_cursor = cur_warp_draw;
			game->move_cursor = cur_warp_move;
			break;
		case CUR_ROW:
			game->cursor_init = cur_row_init;
			game->shift = cur_row_shift;
			game->draw_cursor = cur_row_draw;
			game->move_cursor = cur_row_move;
			break;
		case CUR_COL:
			game->cursor_init = cur_col_init;
			game->shift = cur_col_shift;
			game->draw_cursor = cur_col_draw;
			game->move_cursor = cur_col_move;
			break;
		case CUR_SNAKE:
			game->cursor_init = cur_snake_init;
			game->shift = cur_snake_shift;
			game->draw_cursor = cur_snake_draw;
			game->move_cursor = cur_snake_move;
			break;
		case CUR_RING:
			game->cursor_init = cur_ring_init;
			game->shift = cur_ring_shift;
			game->draw_cursor = cur_ring_draw;
			game->move_cursor = cur_ring_move;
			break;
		case CUR_TWOBYTWO:
			game->cursor_init = cur_2x2_init;
			game->shift = cur_2x2_shift;
			game->draw_cursor = cur_2x2_draw;
			game->move_cursor = cur_2x2_move;
			break;
		case CUR_HSPLIT:
			game->cursor_init = cur_hsplit_init;
			game->shift = cur_hsplit_shift;
			game->draw_cursor = cur_hsplit_draw;
			game->move_cursor = cur_hsplit_move;
			break;
		default:
			break;
	}

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

void game_loop(struct Context* ctx) {
	struct Game* game = &ctx->game;
	struct Input* input = &ctx->input;
	struct AudioContext* audio = &ctx->audio;
    
	if(input->quit.just_pressed) {
		fade_transition(ctx, main_menu_init);
		return;
	}

	switch(game->state) {
    	case GAME_PRE:
    		game->yoff_countdown -= 1;
        	if(input->select.just_pressed) {
            	game->state = GAME_ACTIVE;
            	struct Sound sound;
            	sound.priority = 1;
            	sound.callback = snd_move;
            	sound_play(audio, sound);
        	}
        	break;
        case GAME_ACTIVE:
        	game_control(game, input, audio);
        	game_tick(game, audio);
            break;
		case GAME_POST_HITCH:
			game->yoff_countdown--;
    		break;
    	case GAME_POST_DISPLAY:
	    	game->yoff_countdown--;
        	if(input->select.just_pressed) {
            	game_init(game);
            	struct Sound sound;
            	sound.priority = 1;
            	sound.callback = snd_move;
            	sound_play(audio, sound);
            	game->state = GAME_ACTIVE;
        	}
	    	break;
    	default:
        	break;
	}
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
		game->state = GAME_POST_HITCH;
		game->yoff_countdown = 250; // used for state handling
		sound_play_new(audio, snd_lose, 1, NULL);
		return;
	} 		

	// Player managed to clear up top of the board before the grace period expired.
	game->grace_period = false;
	goto generate_new_row;

iterate_yoff:
	game->yoff_countdown = speeds[game->speed];

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
	if(game->speed_countdown == 0 && game->speed < 19) {
		game->speed++;
		game->speed_countdown = game->speed_increase_interval * 1250;
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
