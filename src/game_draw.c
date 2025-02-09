#include "game_draw.h"

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
