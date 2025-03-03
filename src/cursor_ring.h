#ifndef cursor_ring_h_INCLUDED
#define cursor_ring_h_INCLUDED

#include "game.h"

void cur_ring_init(struct Game* game);
bool cur_ring_shift(struct Game* game);
void cur_ring_draw(struct Game* game, struct DrawContext* ctx);
bool cur_ring_move(struct Game* game, struct Input* input);

#endif // cursor_ring_h_INCLUDED
