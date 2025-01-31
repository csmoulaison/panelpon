#ifndef match_h_INCLUDED
#define match_h_INCLUDED

#define BOARD_WIDTH 8
#define BOARD_HEIGHT 16
#define BOARD_LEN BOARD_WIDTH * BOARD_HEIGHT

struct {
    uint8_t cursor_x;
    uint8_t cursor_y;
	uint8_t board[BOARD_LEN];
} typedef Match;

#endif // match_h_INCLUDED
