#include "game_draw.h"

void game_draw(struct Game* game, struct DrawContext* ctx) {
	ctx->xoff = LOGICAL_W / 2 - BOARD_W / 2 * 8 - 16;
	ctx->yoff = LOGICAL_H / 2 - BOARD_H / 2 * 8;

	switch(game->state) {
		case GAME_ACTIVE:
			game_draw_active(game, ctx);
			break;
		case GAME_PRE:
			game_draw_pre(game, ctx);
			break;
		case GAME_POST_HITCH:
			game_draw_post_hitch(game, ctx);
			break;
		case GAME_POST_DISPLAY:
			game_draw_post_display(game, ctx);
			break;
		default:
			break;
	}

	// Draw border
	draw_fill_rect(ctx, (struct IRect){0, BOARD_H * 8 - 1, BOARD_W * 8, 10}, PL_ALL_BLACK);
	draw_rect(ctx, (struct IRect){-3, -3, BOARD_W * 8 + 4, BOARD_H * 8 + 3}, PL_ALL_WHITE);

	// Draw score text
	draw_set_font(ctx, FONT_BIG);
	
	uint8_t lmarg = BOARD_W * 8 + 6;
	draw_text(ctx, "score", lmarg, 0, PL_ALL_WHITE, JUSTIFY_L);
	char num_str[8];
	sprintf(num_str, "%i", game->score_visible);
	struct Pallete str_pl = PL_YELLOW;
	if((game->score_blink / 4) % 2 != 0) {
		str_pl = PL_ALL_WHITE;
	}
	draw_text(ctx, num_str, lmarg, 10, str_pl, JUSTIFY_L);

	// Draw speed text
	draw_text(ctx, "speed", lmarg, 30, PL_ALL_WHITE, JUSTIFY_L);
	sprintf(num_str, "%i", game->speed + 1);
	str_pl = PL_CYAN;
	if((game->speed_blink / 4) % 2 != 0) {
		str_pl = PL_ALL_WHITE;
	}
	draw_text(ctx, num_str, lmarg, 40, str_pl, JUSTIFY_L);

	// Draw portrait
	draw_rect(ctx, (struct IRect){lmarg, BOARD_H * 8 - 32 + 2, 30, 30}, PL_ALL_WHITE);
	draw_sprite(ctx, (struct IRect){290, 2, 28, 28}, lmarg + 2, BOARD_H * 8 - 32 + 4, PL_PURPLE);
	draw_set_font(ctx, FONT_SMALL);
	draw_text(ctx, "isabel", lmarg, BOARD_H * 8 - 36, PL_PURPLE, JUSTIFY_L);
}

void game_draw_active(struct Game* game, struct DrawContext* ctx) {
	// Skip empty tiles during tile draw below
	bool skips[BOARD_LEN];
	for(uint8_t i = 0; i < BOARD_LEN; i++) {
		skips[i] = game->tiles[i] == 0;
	}

	// Draw shifts
	for(uint8_t i = 0; i < BOARD_LEN; i++) {
		uint8_t t = game->shifts[i].t;
		uint8_t from = i;
		uint8_t to = game->shifts[i].to_pos;

		// Don't draw if from position is empty or enough t has elapsed
		if(t < (FRAMES_SHIFT / 2) + 1 || game->tiles[from] == 0) {
			continue;
		}

		// Skip flipping tiles during tile draw below
		skips[i] = true;

		// Get sprite offset and orientation based on adjacency of the other tile.
		bool vert = false;
		bool warp = false;
		bool altpos = false; // used to determine "anchor" point for drawing flip anim
		SDL_RendererFlip flip;
		if(to == xoffset(from, -1)) {
			flip = SDL_FLIP_NONE;
			altpos = true;
		} else if(to == xoffset(from, 1)) {
			flip = SDL_FLIP_HORIZONTAL;
		} else if(to == yoffset(from, -1)) {
			vert = true;
			altpos = true;
			flip = SDL_FLIP_NONE;
		} else if(to == yoffset(from, 1)) {
			vert = true;
			flip = SDL_FLIP_VERTICAL;
		} else {
			warp = true;
		}

		// TODO - Time to rework animations and the way we get sprites to better
		// reflect needs.
		struct IRect spr;
		struct Pallete pl;
		spr_from_index(game->tiles, from, &spr, &pl);
		float anim_t = FRAMES_SHIFT - t;

		if(warp) {
			spr.x = SPR_TILE_MOVE_WARP_OFFSET;
			spr.y = 0;
			draw_anim_flip(ctx, anim_t, SPR_TILE_MOVE_FRAME_LEN, spr, xcoord(from) * 8, 8 - game->yoff + ycoord(from) * 8, pl, flip);

			spr_from_index(game->tiles, to, &spr, &pl);
			spr.x = SPR_TILE_MOVE_WARP_OFFSET;
			spr.y = 0;
			draw_anim_flip(ctx, anim_t, SPR_TILE_MOVE_FRAME_LEN, spr, xcoord(to) * 8, 8 - game->yoff + ycoord(to) * 8, pl, flip);
			continue;
		}

		// NOTE drawx,drawy as given here only applies to directly adjacent swaps, so
		// we will eventually need further logic for warps, for instance.
		uint8_t drawx = xcoord(from);
		uint8_t drawy = ycoord(from);
		if(altpos) {
			drawx = xcoord(to);
			drawy = ycoord(to);
		}

		if(vert) {
			spr.x = SPR_TILE_MOVE_VERT_OFFSET + game->tiles[from] * 8;
			spr.y = 0;
			spr.h *= 2;
			draw_anim_flip_vert(ctx, anim_t, SPR_TILE_MOVE_FRAME_LEN, spr, drawx * 8, 8 - game->yoff + drawy * 8, pl, flip);
		} else {
			spr.x = SPR_TILE_MOVE_OFFSET;
			spr.y = game->tiles[from] * 8;
			spr.w *= 2;
			draw_anim_flip(ctx, anim_t, SPR_TILE_MOVE_FRAME_LEN, spr, drawx * 8, 8 - game->yoff + drawy * 8, pl, flip);
		}
	}

	// Draw regular/falling tiles - skipping skips
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
		//if(shifting(game, i)) draw_sprite(ctx, SPR_DEBUG_SHIFT, xcoord(i) * 8, 8 - game->yoff + ycoord(i) * 8, PL_YELLOW);		
		//if(matching(game, i)) draw_sprite(ctx, SPR_DEBUG_MATCH, xcoord(i) * 8, 8 - game->yoff + ycoord(i) * 8, PL_BLUE);		
		//if(falling(game, i)) draw_sprite(ctx, SPR_DEBUG_FALL, xcoord(i) * 8, 8 - game->yoff + ycoord(i) * 8, PL_GREEN);
		//if(fall_buffered(game, i)) draw_sprite(ctx, SPR_DEBUG_BUF, xcoord(i) * 8, 8 - game->yoff + ycoord(i) * 8, PL_RED);
	}

}

void game_draw_pre(struct Game* game, struct DrawContext* ctx) {
	struct Pallete pl = fancy_blink_pl(game->yoff_countdown, 8, 3, PL_ALL_BLACK, PL_RED, PL_PURPLE);

	draw_set_font(ctx, FONT_BIG);
	uint8_t center = (BOARD_W * 8) / 2;
	draw_text(ctx, "press", center, 16, pl, JUSTIFY_C);
	draw_text(ctx, "enter", center, 28, pl, JUSTIFY_C);
}

void game_draw_post_hitch(struct Game* game, struct DrawContext* ctx) {
	if(game->yoff_countdown == 0) {
		game->state = GAME_POST_DISPLAY;
	}
	
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

void game_draw_post_display(struct Game* game, struct DrawContext* ctx) {
	uint8_t center = (BOARD_W * 8) / 2;

	draw_set_font(ctx, FONT_BIG);
	struct Pallete game_over_pls[2] = {PL_RED, PL_BLUE};
	for(int8_t y = 0; y < 2; y++) {
		draw_text(ctx, "game", center, 16 - y, game_over_pls[y], JUSTIFY_C);
		draw_text(ctx, "over", center, 28 - y, game_over_pls[y], JUSTIFY_C);
	}

	draw_set_font(ctx, FONT_SMALL);

	struct Pallete blink_pl = fancy_blink_pl(game->yoff_countdown, 16, 3, PL_BLUE, PL_BLUE, PL_CYAN);
	draw_text(ctx, "escape to", center, 48, PL_CYAN, JUSTIFY_C);
	blink_pl = fancy_blink_pl(game->yoff_countdown, 16, 3, PL_ALL_BLACK, PL_BLUE, PL_CYAN);
	draw_text(ctx, "quit", center, 55, blink_pl, JUSTIFY_C);

	blink_pl = fancy_blink_pl(game->yoff_countdown + 64, 16, 3, PL_BLUE, PL_BLUE, PL_CYAN);
	draw_text(ctx, "enter to", center, 66, PL_CYAN, JUSTIFY_C);
	blink_pl = fancy_blink_pl(game->yoff_countdown + 64, 16, 3, PL_ALL_BLACK, PL_BLUE, PL_CYAN);
	draw_text(ctx, "retry", center, 73, blink_pl, JUSTIFY_C);
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
