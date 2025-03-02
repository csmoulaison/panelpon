#ifndef cursor_snake_h_INCLUDED
#define cursor_snake_h_INCLUDED

#include "game.h"

void cur_snake_init(struct Game* game);
bool cur_snake_shift(struct Game* game);
void cur_snake_draw(struct Game* game, struct DrawContext* ctx);
bool cur_snake_move(struct Game* game, struct Input* input);

#endif // cursor_snake_h_INCLUDED
