#ifndef main_menu_h_INCLUDED
#define main_menu_h_INCLUDED

#include "menu.h"

void main_menu_init(struct Context* ctx);
void main_menu_loop(struct Menu* menu, struct Context* ctx);
void main_menu_draw(struct Menu* menu, struct DrawContext* ctx);

#endif // main_menu_h_INCLUDED
