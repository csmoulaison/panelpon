#ifndef input_h_INCLUDED
#define input_h_INCLUDED

#include "SDL2/SDL.h"
#include "stdbool.h"

#define MAX_SCANCODE_BTN_MAPS 64
#define MAX_BTNS 32

struct {
	bool held;
	bool just_pressed;
	bool just_released;
} typedef Button;

struct {
    SDL_Scancode sc;
    Button* btn;
} typedef ScancodeButtonMap;

struct {
	ScancodeButtonMap scancode_btn_maps[MAX_SCANCODE_BTN_MAPS];
	int scancode_btn_maps_len;

	Button* mapped_btns[MAX_BTNS];
	int mapped_btns_len;

	Button quit;
	Button up;
	Button down;
	Button left;
	Button right;
	Button select;

	bool quit_event;
} typedef Input;

void input_poll(Input* input);
void map_scancode_to_button(Input* input, SDL_Scancode sc, Button* btn);

#endif // input_h_INCLUDED
