#ifndef game_draw_h_INCLUDED
#define game_draw_h_INCLUDED

#include "game.h"
#include "board.h"
#include "draw.h"

void game_draw(struct Game* game, struct DrawContext* ctx);
void game_draw_active(struct Game* game, struct DrawContext* ctx);
void game_draw_pre(struct Game* game, struct DrawContext* ctx);
void game_draw_post(struct Game* game, struct DrawContext* ctx);

void spr_from_index(uint8_t* board, uint8_t i, struct IRect* spr, struct Pallete* pl);

#endif // game_draw_h_INCLUDED
