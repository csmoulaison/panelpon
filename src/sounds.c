#include "sounds.h"

#include "math.h"
#include "stdio.h"

#define PI 3.14159265358979323846

void snd_flip(struct Sound* sound) {
	float len = 0.2;
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

	sound->freq = 200 - tmod * 100;
	sound->amp = 0.0 + tmod * 0.3;

	if(sound->t > len) sound->active = false;
}

void snd_match(struct Sound* sound) {
	float len = 0.3;
	float tmod = sound->t / len;
    float tsin = sin((sound->t * PI * 8) / len);

	sound->freq = 400 + tsin * 400 + tmod * 800;
	sound->amp = 0.00 + tsin * 0.3;

	if(sound->t > len) sound->active = false;
}

void snd_lose(struct Sound* sound) {
    float len = 3;
    float tmod = sound->t / len;

	tmod /= 2;
	sound->freq = 200 - ((int)(tmod * 200) % 3) * (tmod * 400);
	tmod *= 2;
	sound->amp = 0.45 - tmod * 0.45;

	if(sound->t > len) sound->active = false;
}
