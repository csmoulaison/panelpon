#include "menu.h"

#include "sounds.h"
#include "config.h"
#include "context.h"

void menu_control(struct Menu* menu, struct Context* ctx, bool decr, bool incr) {
	struct AudioContext* audio = &ctx->audio;

    menu->clock++; // used for some visual effects

	if(incr) {
		menu->selected++;
		if(menu->selected >= menu->len) {
			menu->selected = 0;
		}
		sound_play_new(audio, snd_up, 1, NULL);
		menu->clock = 2;
	} else if(decr) {
		menu->selected--;
		if(menu->selected == 255) {
			menu->selected = menu->len - 1;
		}
		sound_play_new(audio, snd_down, 1, NULL);
		menu->clock = 0;
	}
}
