#include "transition.h"

#include "sounds.h"

void fade_transition(struct Context* ctx, void(*callback)(struct Context* ctx)) {
	ctx->transition_t = 40;
	ctx->transition_callback = callback;
    sound_play_new(&ctx->audio, snd_transition, 1, NULL);
}

struct Pallete pl_from_transition_state(struct Pallete pl, uint8_t state) {
	if(state == 0) {
		return pl;
	}

	if(state == 3) {
		return PL_ALL_BLACK;
	}

	uint32_t* ptr = (uint32_t*)&pl;
	for(uint8_t i = 0; i < 2; i++) {
		if(state == 2) {
			goto transition_state_2;
		}
		switch(ptr[i]) {
			case 0xFFFFFF:
				ptr[i] = 0xFF00FF;
				break;
			case 0x0000FF:
				ptr[i] = 0xFF0000;
				break;
			case 0x00FF00:
				ptr[i] = 0xFF0000;
				break;
			case 0xFF0000:
				ptr[i] = 0xFF0000;
				break;
			case 0xFF00FF:
				ptr[i] = 0xFF0000;
				break;
			case 0x00FFFF:
				ptr[i] = 0x0000FF;
				break;
			case 0xFFFF00:
				ptr[i] = 0xFF0000;
				break;
		}
		continue;
transition_state_2:
		switch(ptr[i]) {
			case 0xFFFFFF:
				ptr[i] = 0xFF0000;
				break;
			case 0x0000FF:
				ptr[i] = 0x000000;
				break;
			case 0x00FF00:
				ptr[i] = 0x000000;
				break;
			case 0x000000:
				ptr[i] = 0x000000;
				break;
			case 0xFF00FF:
				ptr[i] = 0x000000;
				break;
			case 0x00FFFF:
				ptr[i] = 0x000000;
				break;
			case 0xFFFF00:
				ptr[i] = 0x000000;
				break;
		}
	}
	return pl;
}
