#ifndef cursor_hsplit_h_INCLUDED
#define cursor_hsplit_h_INCLUDED

#include "game.h"

void cur_hsplit_init(struct Game* game);
bool cur_hsplit_shift(struct Game* game);
void cur_hsplit_draw(struct Game* game, struct DrawContext* ctx);
bool cur_hsplit_move(struct Game* game, struct Input* input);

#endif // cursor_hsplit_h_INCLUDED
