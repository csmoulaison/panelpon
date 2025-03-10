#ifndef menu_h_INCLUDED
#define menu_h_INCLUDED

#include "stdbool.h"
#include "stdint.h"
#include "draw.h"
#include "cursor_type.h"

struct Context; // To deal with circular dependency

struct Menu {
	uint8_t selected;
	uint8_t len;
	uint8_t clock;
};

void menu_control(struct Menu* menu, struct Context* ctx, bool decr, bool incr);

#endif // menu_h_INCLUDED
