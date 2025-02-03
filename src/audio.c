#include "audio.h"

#include "stdlib.h"
#include "stdio.h"

#define AUDIO_BUF_LEN 2048

void audio_data_callback(ma_device* device, void* output, const void* input, uint32_t frames_len) {
    AudioCallbackData* data = (AudioCallbackData*)device->pUserData;
    float* fout= (float*)output;
	float* buf[AUDIO_BUF_LEN];

	// Madness. See note on deprecation in audio.h
	ma_waveform_read_pcm_frames(&data->waveform, buf, (uint64_t)frames_len, NULL);

	for(int i = 0; i < WAVES_LEN; i++) {
		float delta = (1.0 / SAMPLE_RATE) * data->waves[i].freq;

		for(int j = 0; j < frames_len; j++) {
    		data->waves[i].t += delta;

			// Calculate square wave based on wave.t
			float* frame = fout + j;
			if((int)data->waves[i].t % 2 == 0) {
    			*frame += data->waves[i].amp;
    			if(*frame > 0.99) {
					*frame = 0.99;
    			} 
			} else {
    			*frame -= data->waves[i].amp;
    			if(*frame < -0.99) {
					*frame = -0.99;
    			} 
			}

			// Re-up floating point accuracy
			if(data->waves[i].t > 64) {
    			data->waves[i].t -= 64;
			}
		}
	}
}
