#ifndef menu_h_INCLUDED
#define menu_h_INCLUDED

#include "stdbool.h"
#include "stdint.h"
#include "draw.h"

struct Context; // To deal with circular dependency

struct Menu {
	bool just_selected;
	uint8_t selection;
	const char** options;
	uint8_t len;
	void (*callback)(struct Menu* menu, struct Context* ctx);
};

void menu_control(struct Menu* menu, struct Context* ctx);
void menu_draw(struct Menu* menu, struct DrawContext* ctx);

#endif // menu_h_INCLUDED
