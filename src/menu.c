#include "menu.h"

#include "sounds.h"
#include "config.h"
#include "context.h"

void menu_control(struct Menu* menu, struct Context* ctx) {
	struct Input* input = &ctx->input;
	struct AudioContext* audio = &ctx->audio;
    
	if(input->down.just_pressed) {
		menu->selected++;
		if(menu->selected >= menu->len) {
			menu->selected = 0;
		}
		sound_play_new(audio, snd_up, 1, NULL);
	} else if(input->up.just_pressed) {
		menu->selected--;
		if(menu->selected == 255) {
			menu->selected = menu->len - 1;
		}
		sound_play_new(audio, snd_down, 1, NULL);
	}
}
