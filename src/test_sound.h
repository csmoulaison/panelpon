#ifndef test_sound_h_INCLUDED
#define test_sound_h_INCLUDED

#include "audio.h"

enum SoundState test(struct Voice* voice, void* data) {
	if(voice->t > 1) {
		return SOUND_FINISHED;
	}

	voice->freq = *(float*)data;
	voice->amp = 0.2;
	return SOUND_ONGOING;
}

#endif // test_sound_h_INCLUDED
