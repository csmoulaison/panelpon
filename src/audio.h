#ifndef audio_h_INCLUDED
#define audio_h_INCLUDED

#include "portaudio.h"
#include "stdint.h"
#include "stdbool.h"

#define VOICES_LEN 2
#define SAMPLE_RATE 48000
#define SOUNDSTACK_SIZE 8
#define SOUNDS_MAX 16
#define AUDIO_BUF_LEN 1024

struct Sound {
    // Must be set on play
    bool active;
	uint8_t priority;
	void* data;
	void (*callback)(struct Sound* sound);

	// Intended to be dynamically mofified by callback
	float amp;
	float freq;
	double t;
};

struct Voice {
	struct Sound soundstack[SOUNDSTACK_SIZE];
	uint8_t soundstack_len;
};

struct AudioContext {
    // Data sent to audio callback
    struct Voice voices[VOICES_LEN];
};

void audio_update(struct AudioContext* ctx);
int audio_callback(const void* input, void* output, unsigned long frames_len, const PaStreamCallbackTimeInfo* time_info, PaStreamCallbackFlags status_flags, void* userdata);
void sound_play(struct AudioContext* ctx, struct Sound sound);

#endif // audio_h_INCLUDED
