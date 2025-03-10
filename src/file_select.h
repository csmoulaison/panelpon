#ifndef file_select_h_INCLUDED
#define file_select_h_INCLUDED

#include "menu.h"
#include "game.h"

void fselect_menu_init(struct Menu* menu, struct Game* game);
void fselect_menu_loop(struct Menu* menu, struct Context* ctx);
void fselect_menu_draw(struct Menu* menu, struct Game* game, struct DrawContext* ctx);

#endif // file_select_h_INCLUDED
