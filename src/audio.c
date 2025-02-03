#include "audio.h"

#include "stdlib.h"
#include "stdio.h"

#define AUDIO_BUF_LEN 2048
#define MAX_T_SECONDS 256

#define SOUND_STATE uint8_t
#define SOUND_ONGOING  0
#define SOUND_FINISHED 1

void audio_update(struct AudioContext* ctx, float dt) {
    for(uint8_t i = 0; i < VOICES_LEN; i++) {
        struct Voice* voice = &ctx->data.voices[i];
		if(voice->soundstack_len > 0) {
    		uint8_t id = voice->soundstack[voice->soundstack_len - 1];
    		struct Sound* sound = ctx->library.sounds[id];
			if(sound->callback(voice, &sound->data) == SOUND_FINISHED) {
				voice->soundstack_len--;
			}
		}
    }
}

void audio_data_callback(ma_device* device, void* output, const void* input, uint32_t frames_len) {
    struct AudioData* data = (struct AudioData*)device->pUserData;
    float* fout= (float*)output;
	float* buf[AUDIO_BUF_LEN];

	// Madness. See note on deprecation in audio.h
	ma_waveform_read_pcm_frames(&data->waveform, buf, (uint64_t)frames_len, NULL);

	for(uint8_t i = 0; i < VOICES_LEN; i++) {
		float delta = (1.0 / SAMPLE_RATE) * data->voices[i].freq;

		for(uint8_t j = 0; j < frames_len; j++) {
    		data->voices[i].t += delta;

			// Calculate square wave based on wave.t
			float* frame = fout + j;
			if((uint8_t)data->voices[i].t % 2 == 0) {
    			*frame += data->voices[i].amp;
    			if(*frame > 0.99) {
					*frame = 0.99;
    			} 
			} else {
    			*frame -= data->voices[i].amp;
    			if(*frame < -0.99) {
					*frame = -0.99;
    			} 
			}

			// Re-up floating point accuracy
			if(data->voices[i].t > MAX_T_SECONDS) {
    			data->voices[i].t -= MAX_T_SECONDS;
			}
		}
	}
}

void sound_register(struct SoundLibrary* library, struct Sound* sound) {
    sound->id = library->sounds_len;
    library->sounds[library->sounds_len] = sound;
    library->sounds_len++;
}

void sound_play(struct AudioContext* ctx, struct Sound* sound, struct Voice* voice) {
    // TODO: Don't overflow and check priority
    voice->soundstack[voice->soundstack_len] = sound->id;
    voice->soundstack_len++;
    voice->amp = 0;
    voice->freq = 0;
    voice->t = 0;
}
