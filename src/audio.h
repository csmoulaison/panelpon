#ifndef audio_h_INCLUDED
#define audio_h_INCLUDED

#include "miniaudio/miniaudio.h"
#include "stdint.h"

struct {
	ma_waveform wave;
	uint16_t wave_frequency;
} typedef AudioCallbackData;

struct {
	ma_device device;
	ma_device_config device_config;
	ma_waveform_config wave_config;
	AudioCallbackData callback_data;
} typedef AudioContext;

void audio_data_callback(ma_device* device, void* output, const void* input, uint32_t frames_len);

#endif // audio_h_INCLUDED
