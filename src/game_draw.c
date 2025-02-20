#include "game_draw.h"

void game_draw(struct Game* game, struct DrawContext* ctx) {
	ctx->xoff = LOGICAL_W / 2 - BOARD_W / 2 * 8;
	ctx->yoff = LOGICAL_H / 2 - BOARD_H / 2 * 8;

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
	draw_fill_rect(ctx, (struct IRect){0, BOARD_H * 8 - 1, BOARD_W * 8, 10}, PL_ALL_BLACK);
	draw_rect(ctx, (struct IRect){-3, -3, BOARD_W * 8 + 4, BOARD_H * 8 + 3}, PL_ALL_WHITE);
}

void game_draw_active(struct Game* game, struct DrawContext* ctx) {
	// Skip empty tiles during tile draw below
	bool skips[BOARD_LEN];
	for(uint8_t i = 0; i < BOARD_LEN; i++) {
    	skips[i] = game->tiles[i] == 0;
	}

	// Draw flips
	for(uint8_t i = 0; i < game->swaps_len; i++) {
		if(game->swaps[i].t < (FRAMES_SWAP / 2) + 1) {
    		continue;
		}
    	
		float anim_t = FRAMES_SWAP - game->swaps[i].t;
		uint8_t drawx = xcoord(game->swaps[i].a);
		uint8_t drawy = ycoord(game->swaps[i].a);

		for(int j = 0; j < 2; j++) {
			uint8_t pos = game->swaps[i].positions[j];
			if(game->tiles[pos] == 0) {
    			continue;
			}

			// Get sprite offset and orientation based on adjacency of the other tile.
			// TODO time to rework animations and the way we get sprites to better
			// reflect needs.
    		uint8_t otherpos = game->swaps[i].positions[!j]; // Gives 0 when 1 and 1 when 0. How naughty.
    		SDL_RendererFlip flip;

    		struct IRect spr;
    		spr.w = 8;
    		spr.h = 8;

			bool vert = false;
    		if(otherpos == xoffset(pos, -1)) {
				flip = SDL_FLIP_NONE;
    		} else if(otherpos == xoffset(pos, 1)) {
        		flip = SDL_FLIP_HORIZONTAL;
    		} else if(otherpos == yoffset(pos, -1)) {
        		vert = true;
        		flip = SDL_FLIP_NONE;
    		} else if(otherpos == yoffset(pos, 1)) {
        		vert = true;
        		flip = SDL_FLIP_VERTICAL;
    		}

        	struct IRect nullspr;
        	struct Pallete pl;
        	spr_from_index(game->tiles, pos, &nullspr, &pl);

    		if(!vert) {
				spr.x = SPR_TILE_MOVE_OFFSET;
				spr.y = game->tiles[pos] * 8;
				spr.w *= 2;
            	draw_anim_flip(ctx, anim_t, SPR_TILE_MOVE_FRAME_LEN, spr, drawx * 8, 8 - game->yoff + drawy * 8, pl, flip);
    		} else {
        		printf("vert!\n");
        		spr.x = SPR_TILE_MOVE_VERT_OFFSET + game->tiles[pos] * 8;
        		spr.h *= 2;
            	draw_anim_flip_vert(ctx, anim_t, SPR_TILE_MOVE_FRAME_LEN, spr, drawx * 8, 8 - game->yoff + drawy * 8, pl, flip);
    		}
		}

		// Skip flipping tiles during tile draw below
    	skips[game->swaps[i].a] = true;
    	skips[game->swaps[i].b] = true;
	}

	// Tile draw - skipping skips
    for(uint8_t i = 0; i < BOARD_LEN; i++) {
		if(skips[i]) {
    		continue;
		}
        
    	struct IRect spr;
    	struct Pallete pl;
    	spr_from_index(game->tiles, i, &spr, &pl);

		// Offset y if currently falling
		uint8_t yoff = 0;
        if(game->falls[i] != 0) {
			yoff = (game->falls[i] * 8) / FRAMES_FALL;
        }

        if(game->matches[i] != 0 && (game->matches[i] / 12) % 2 != 0) {
        	pl = PL_ALL_WHITE;
		}
		if(ycoord(i) == BOARD_H - 1) {
    		//pl = PL_ALL_WHITE;
    		spr.x += SPR_TILE_INACTIVE_OFFSET;
		}
		
        draw_sprite(ctx, spr, xcoord(i) * 8, 8 - game->yoff + ycoord(i) * 8 - yoff, pl);
	}

	// Cursor
	game->draw_cursor(game, ctx);

	// Draw debug gizmos
	for(int i = 0; i < BOARD_LEN; i++) {
		//if(swapping(game, i)) draw_sprite(ctx, SPR_DEBUG_SWAP, xcoord(i) * 8, 8 - game->yoff + ycoord(i) * 8, PL_YELLOW);		
		//if(matching(game, i)) draw_sprite(ctx, SPR_DEBUG_MATCH, xcoord(i) * 8, 8 - game->yoff + ycoord(i) * 8, PL_BLUE);		
		//if(falling(game, i)) draw_sprite(ctx, SPR_DEBUG_FALL, xcoord(i) * 8, 8 - game->yoff + ycoord(i) * 8, PL_GREEN);
		//if(fall_buffered(game, i)) draw_sprite(ctx, SPR_DEBUG_BUF, xcoord(i) * 8, 8 - game->yoff + ycoord(i) * 8, PL_RED);
	}
}

void game_draw_pre(struct Game* game, struct DrawContext* ctx) {
	(void)game;
	(void)ctx;
}

void game_draw_post(struct Game* game, struct DrawContext* ctx) {
    for(uint8_t i = 0; i < BOARD_LEN; i++) {  
		if(game->tiles[i] == 0) {
    		continue;
		}

    	struct IRect spr;
    	struct Pallete pl;
    	spr_from_index(game->tiles, i, &spr, &pl);

		// Offset y if currently falling
		uint8_t yoff = 0;
        if(game->falls[i] != 0) {
			yoff = (game->falls[i] * 8) / FRAMES_FALL;
        }

        if((game->yoff_countdown / 16) % 2 != 0) {
        	pl = PL_ALL_WHITE;
		}
        draw_sprite(ctx, spr, xcoord(i) * 8, 8 - game->yoff + ycoord(i) * 8 - yoff, pl);
	}
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
