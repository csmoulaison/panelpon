#ifndef menu_draw_h_INCLUDED
#define menu_draw_h_INCLUDED

#include "stdint.h"
#include "menu.h"
#include "draw.h"

void menu_draw_list(struct DrawContext* ctx, struct Menu* menu, const char** opt_strs);

#endif // menu_draw_h_INCLUDED
