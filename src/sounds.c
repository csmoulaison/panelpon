#include "sounds.h"

#include "math.h"

#define PI 3.14159265358979323846

void snd_flip(struct Sound* sound) {
	float len = 0.1;
    float tmod = sin((sound->t * PI * 2) / len);

	sound->freq = 400 + tmod * 400;
	sound->amp = 0.00 + tmod * 0.3;

	if(sound->t > len) sound->active = false;
}

void snd_noflip(struct Sound* sound) {
    float len = 0.1;
    float tmod = sound->t / len;

	sound->freq = 200 + tmod * -200;
	sound->amp = 0.3 - tmod * 0.3;

	if(sound->t > len) sound->active = false;
}

void snd_move(struct Sound* sound) {
    float len = 0.1;
    float tmod = sound->t / len;

	sound->freq = 200 - tmod * 100 ;
	sound->amp = 0.00 + tmod * 0.3;

	if(sound->t > len) sound->active = false;
}
