#ifndef test_sound_h_INCLUDED
#define test_sound_h_INCLUDED

#include "audio.h"

void test(struct Sound* sound) {
	sound->freq = *(float*)sound->data + sound->t * 1000;
	sound->amp = 0.2;

	if(sound->t > 0.1) {
    	sound->active = false;
	}
}

#endif // test_sound_h_INCLUDED
