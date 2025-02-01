#ifndef match_h_INCLUDED
#define match_h_INCLUDED

#define BOARD_W 8
#define BOARD_H 16
#define BOARD_LEN BOARD_W * BOARD_H

struct {
    uint8_t curx;
    uint8_t cury;
	uint8_t board[BOARD_LEN];
} typedef Match;

#endif // match_h_INCLUDED
