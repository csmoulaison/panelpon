#include "loop.h"

#include "sprites.h"

void loop(Context* ctx) {
    // Calculate delta time
	ctx->time_last = ctx->time_now;	
	ctx->time_now = SDL_GetTicks64();
	ctx->dt = (double)(ctx->time_now - ctx->time_last) / 1000;

	// Poll input
	input_poll(&ctx->input);

	// Start draw cycle
	draw_clear(ctx);

	// Simulate
	if(ctx->input.up.just_pressed) {
		ctx->match.cury -= 1;
	}
	if(ctx->input.down.just_pressed) {
		ctx->match.cury += 1;
	}
	if(ctx->input.left.just_pressed) {
		ctx->match.curx -= 1;
	}
	if(ctx->input.right.just_pressed) {
		ctx->match.curx += 1;
	}

	if(ctx->match.cury == 255) {
    	ctx->match.cury = 0;
	}
	// TODO: why - 1 on H and - 2 on W?
	if(ctx->match.cury > BOARD_H - 1) {
    	ctx->match.cury = BOARD_H - 1;
	}
	if(ctx->match.curx == 255) {
    	ctx->match.curx = 0;
	}
	if(ctx->match.curx > BOARD_W - 2) {
    	ctx->match.curx = BOARD_W - 2;
	}

	if(ctx->input.select.just_pressed) {
		uint8_t i = ctx->match.cury * BOARD_W + ctx->match.curx;
		uint8_t tmp = ctx->match.board[i];
		ctx->match.board[i] = ctx->match.board[i + 1];
		ctx->match.board[i + 1] = tmp;
	}
	
	// Draw
	uint8_t board_x = LOGICAL_W / 2 - BOARD_W / 2 * 8;
	uint8_t board_y = LOGICAL_H / 2 - BOARD_H / 2 * 8;
	for(uint8_t y = 0; y < BOARD_H; y++) {
    	for(uint8_t x = 0; x < BOARD_W; x++) {
        	uint8_t i = y * BOARD_W + x;

        	IRect spr;
        	Pallete pl;
        	switch(ctx->match.board[i]) {
            case 0:
                spr = SPR_ROUND;
                pl = PL_BLUE;
                break;
            case 1:
                spr = SPR_SQUARE;
                pl = PL_RED;
                break;
            case 2:
                spr = SPR_DIAMOND;
                pl = PL_GREEN;
                break;
            case 3:
                spr = SPR_CROSS;
                pl = PL_PURPLE;
                break;
            default:
                break;
        	}

            draw_sprite(ctx, spr, (IRect){board_x + x * 8, board_y + y * 8, 8, 8}, pl);
    	}
	}

	draw_sprite(ctx, SPR_CURSOR, (IRect){board_x + ctx->match.curx * 8 - 1, board_y + ctx->match.cury * 8 - 1, 18, 10}, PL_ALL_WHITE);
	
	draw_present(ctx);
}
