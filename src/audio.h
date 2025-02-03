#ifndef audio_h_INCLUDED
#define audio_h_INCLUDED

#define MA_NO_ENCODING
#define MA_NO_DECODING
#define MINIAUDIO_IMPLEMENTATION
#include "minisucks/miniaudio.h"
#include "stdint.h"

#define VOICES_LEN 2
#define SAMPLE_RATE 48000
#define SOUNDSTACK_SIZE 8
#define SOUNDS_MAX 16

enum SoundState {
    SOUND_ONGOING = 0,
    SOUND_FINISHED = 1
};

struct Voice {
    // Waveform state
	float amp;
	float freq;
	float t;

	uint8_t soundstack[SOUNDSTACK_SIZE];
	uint8_t soundstack_len;
};

struct Sound{
    // Overriden on registration
	uint8_t id;

	// Defined at registration site
	uint8_t priority;
	void* data;
	enum SoundState (*callback)(struct Voice* voice, void* data);
};

struct SoundLibrary{
	struct Sound* sounds[SOUNDS_MAX];
	uint8_t sounds_len;
};

struct AudioData {
    struct Voice voices[VOICES_LEN];

    // TODO: see note below on deprecating this
	ma_waveform waveform;
};

struct AudioContext {
    // Voice/miniaudio state
	ma_device device;
	ma_device_config device_config;
	struct AudioData data;
	// Only used to read PCM frames.
	// TODO: read up on alternate solutions, because this is embarrasing.
	ma_waveform_config wave_config;

	// Sounds
	struct SoundLibrary library;
};

void audio_update(struct AudioContext* ctx, float dt);
void audio_data_callback(ma_device* device, void* output, const void* input, uint32_t frames_len);
void sound_register(struct SoundLibrary* library, struct Sound* sound);
void sound_play(struct AudioContext* ctx, struct Sound* sound, struct Voice* voice);

#endif // audio_h_INCLUDED
