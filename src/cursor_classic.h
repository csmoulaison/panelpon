#ifndef cursor_classic_h_INCLUDED
#define cursor_classic_h_INCLUDED

#include "game.h"

void cur_classic_init(struct Game* game);
bool cur_classic_shift(struct Game* game);
void cur_classic_draw(struct Game* game, struct DrawContext* ctx);
bool cur_classic_move(struct Game* game, struct Input* input);

#endif // cursor_classic_h_INCLUDED
