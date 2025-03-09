#ifndef main_menu_h_INCLUDED
#define main_menu_h_INCLUDED

#include "stdbool.h"

#include "input.h"
#include "audio.h"
#include "draw.h"
#include "game.h"

enum MenuType {
	MENU_MAIN
};

struct Menu {
	enum MenuType type;
	bool just_selected;
	uint8_t selection;
	uint8_t len;
};

enum ProgState menu_control(struct Menu* menu, struct Input* input, struct AudioContext* audio, struct Game* game);
void menu_draw(struct Menu* menu, struct DrawContext* ctx);

#endif // main_menu_h_INCLUDED
