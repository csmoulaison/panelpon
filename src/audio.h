#ifndef audio_h_INCLUDED
#define audio_h_INCLUDED

#define MA_NO_DECODING
#define MA_NO_ENCODING
#include "miniaudio/miniaudio.h"
#include "stdint.h"

#define WAVES_LEN 2
#define SAMPLE_RATE 48000

struct {
	float amp;
	float freq;
	float t;
} typedef Wave;

struct {
    Wave waves[WAVES_LEN];

    // TODO: see note below on deprecating this
	ma_waveform waveform;
} typedef AudioCallbackData;

struct {
	ma_device device;
	ma_device_config device_config;
	AudioCallbackData data;

	// Only used to read PCM frames.
	// TODO: read up on alternate solutions, because this is embarrasing.
	ma_waveform_config wave_config;
} typedef AudioContext;

void audio_data_callback(ma_device* device, void* output, const void* input, uint32_t frames_len);

#endif // audio_h_INCLUDED
