#ifndef cursor_vert_h_INCLUDED
#define cursor_vert_h_INCLUDED

#include "game.h"

uint8_t cur_vert_start_pos(struct Game* game);
bool cur_vert_shift(struct Game* game);
void cur_vert_draw(struct Game* game, struct DrawContext* ctx);
uint8_t cur_vert_move(struct Game* game, struct Input* input);

#endif // cursor_vert_h_INCLUDED
