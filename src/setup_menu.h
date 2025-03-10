#ifndef setup_menu_h_INCLUDED
#define setup_menu_h_INCLUDED

#include "menu.h"
#include "game.h"

void setup_menu_init(struct Menu* menu, struct Game* game);
void setup_menu_loop(struct Menu* menu, struct Context* ctx);
void setup_menu_draw(struct Menu* menu, struct Game* game, struct DrawContext* ctx);

#endif // setup_menu_h_INCLUDED
