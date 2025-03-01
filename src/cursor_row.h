#ifndef cursor_row_h_INCLUDED
#define cursor_row_h_INCLUDED

#include "game.h"

void cur_row_init(struct Game* game);
bool cur_row_shift(struct Game* game);
void cur_row_draw(struct Game* game, struct DrawContext* ctx);
bool cur_row_move(struct Game* game, struct Input* input);

#endif // cursor_row_h_INCLUDED
