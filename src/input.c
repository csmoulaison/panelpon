#include "input.h"
#include "stdio.h"

void input_poll(Input* input)  {
	for(int i = 0; i < input->mapped_btns_len; i++) {
		input->mapped_btns[i]->just_pressed  = false;
		input->mapped_btns[i]->just_released = false;
	}
	
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        switch(e.type) {
        case SDL_QUIT:
            input->quit_event = true;
            break;
        case SDL_WINDOWEVENT:
            if(e.window.event == SDL_WINDOWEVENT_CLOSE) {
                input->quit_event = true;
            }
            break;
        case SDL_KEYDOWN:
	        for(int i = 0; i < input->scancode_btn_maps_len; i++) {
		        ScancodeButtonMap* map = &input->scancode_btn_maps[i];
                if(e.key.keysym.scancode == map->sc&& !map->btn->held) {
                    map->btn->held = true;
                    map->btn->just_pressed = true;
                }
            }
            break;
        case SDL_KEYUP:
	        for(int i = 0; i < input->scancode_btn_maps_len; i++) {
		        ScancodeButtonMap* map = &input->scancode_btn_maps[i];
                if(e.key.keysym.scancode == map->sc) {
                    if(map->btn->held) {
                        map->btn->just_released = true;
                    }
                    map->btn->held = false;
                }
            }
            break;
        default:
            break;
        }
    }
}

void map_scancode_to_button(Input* input, SDL_Scancode sc, Button* btn) {
    input->scancode_btn_maps[input->scancode_btn_maps_len] = (ScancodeButtonMap){sc, btn};
    input->scancode_btn_maps_len++;

    bool matched = false;
	for(int i = 0; i < input->mapped_btns_len; i++) {
        if(input->mapped_btns[i] == btn) {
            matched = true;
        }
    }

    if(!matched) {
        input->mapped_btns[input->mapped_btns_len] = btn;
        input->mapped_btns_len++;
    }
}
