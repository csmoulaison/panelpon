#ifndef match_h_INCLUDED
#define match_h_INCLUDED

#define MAX_TILE_EVENTS BOARD_LEN

#include "config.h"
#include "input.h"
#include "draw.h"

struct {
	uint8_t tile; // Index into board
	double t; // From 0 to 1
} typedef TileEvent;

struct {
    // Board
	uint8_t board[BOARD_LEN];
	double board_yoff;

	// Cursor
    uint8_t cursor;
    TileEvent cursor_prev;

	// Ongoing board events - some of these are gameplay significant, and some
	// are only for animation.
	TileEvent flips[MAX_TILE_EVENTS]; // flip.tile references left hand tile
	uint8_t flips_len;

	TileEvent explodes[MAX_TILE_EVENTS]; 
	uint8_t explodes_len;

	TileEvent falls[MAX_TILE_EVENTS]; // fall.tile references start y pos
	uint8_t falls_len;
} typedef Match;

void match_control(Match* match, Input* input); 
void match_tick(Match* match, double dt); 
void match_draw(Match* match, DrawContext* ctx);

void coords_from_index(uint8_t i, uint8_t* x, uint8_t* y);
uint8_t index_from_coords(uint8_t x, uint8_t y);
void spr_from_index(uint8_t* board, uint8_t i, IRect* spr, Pallete* pl);

#endif // match_h_INCLUDED
