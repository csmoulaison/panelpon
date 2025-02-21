#ifndef sounds_h_INCLUDED
#define sounds_h_INCLUDED

#include "audio.h"

void snd_shift(struct Sound* sound);
void snd_noshift(struct Sound* sound);
void snd_move(struct Sound* sound);
void snd_match(struct Sound* sound);
void snd_lose(struct Sound* sound);

#endif // sounds_h_INCLUDED
