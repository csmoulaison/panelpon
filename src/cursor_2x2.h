#ifndef cursor_2x2_h_INCLUDED
#define cursor_2x2_h_INCLUDED

#include "game.h"

void cur_2x2_init(struct Game* game);
bool cur_2x2_shift(struct Game* game);
void cur_2x2_draw(struct Game* game, struct DrawContext* ctx);
bool cur_2x2_move(struct Game* game, struct Input* input);

#endif // cursor_2x2_h_INCLUDED
