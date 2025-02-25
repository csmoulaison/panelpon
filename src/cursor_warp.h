#ifndef cursor_warp_h_INCLUDED
#define cursor_warp_h_INCLUDED

#include "game.h"

void cur_warp_init(struct Game* game);
bool cur_warp_shift(struct Game* game);
void cur_warp_draw(struct Game* game, struct DrawContext* ctx);
bool cur_warp_move(struct Game* game, struct Input* input);

#endif // cursor_warp_h_INCLUDED
