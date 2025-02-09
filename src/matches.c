#include "matches.h"

#include "sounds.h"

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
