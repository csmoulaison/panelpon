#ifndef cursor_col_h_INCLUDED
#define cursor_col_h_INCLUDED

#include "game.h"

void cur_col_init(struct Game* game);
bool cur_col_shift(struct Game* game);
void cur_col_draw(struct Game* game, struct DrawContext* ctx);
bool cur_col_move(struct Game* game, struct Input* input);

#endif // cursor_col_h_INCLUDED
