#ifndef input_h_INCLUDED
#define input_h_INCLUDED

#include "SDL2/SDL.h"
#include "stdbool.h"

#define MAX_SCANCODE_BTN_MAPS 64
#define MAX_BTNS 32

struct Button {
	bool held;
	bool just_pressed;
	bool just_released;
};

struct ScancodeButtonMap {
    SDL_Scancode sc;
    struct Button* btn;
};

struct Input {
	struct ScancodeButtonMap scancode_btn_maps[MAX_SCANCODE_BTN_MAPS];
	int scancode_btn_maps_len;

	struct Button* mapped_btns[MAX_BTNS];
	int mapped_btns_len;

	struct Button quit;
	struct Button select;

	struct Button up;
	struct Button down;
	struct Button left;
	struct Button right;

	struct Button up2;
	struct Button down2;
	struct Button left2;
	struct Button right2;

	bool quit_event;
};

void input_poll(struct Input* input);
void map_scancode_to_button(struct Input* input, SDL_Scancode sc, struct Button* btn);

#endif // input_h_INCLUDED
