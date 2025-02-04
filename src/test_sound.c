#include "test_sound.h"

void snd_upbeep(struct Sound* sound) {
	sound->freq = 400 + sound->t * 1000;
	sound->amp = 0.3;

	if(sound->t > 0.1) {
    	sound->active = false;
	}
}

void snd_dark(struct Sound* sound) {
	sound->freq = 100;
	sound->amp = 0.3;

	if(sound->t > 0.05) {
    	sound->active = false;
	}
}
