#include "audio.h"

#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "sounds.h"

#define MAX_T_SECONDS 256

#define SOUND_ONGOING  0
#define SOUND_FINISHED 1

void audio_update(struct AudioContext* ctx) {
    for(uint8_t i = 0; i < VOICES_LEN; i++) {
        struct Voice* voice = &ctx->voices[i];

		// Eliminate inactive sounds on soundstack
		for(int j = 0; j < voice->soundstack_len; j++) {
    		if(!voice->soundstack[j].active) {
				for(int k = j + 1; k < voice->soundstack_len; k++) {
					voice->soundstack[k - 1] = voice->soundstack[k];
				}
				voice->soundstack_len--;
				j--; // We need to recheck the current index now.
    		}
		}

       	// Update sound at the top of the stack
		if(voice->soundstack_len > 0) {
    		struct Sound* sound = &voice->soundstack[voice->soundstack_len - 1];
			sound->callback(sound);
		}
    }
}

int audio_callback(const void* input, void* output, unsigned long frames_per_buf, const PaStreamCallbackTimeInfo* time_info, PaStreamCallbackFlags status_flags, void* userdata) {
    // prevent unused variable warning
    (void)input; 
    (void)time_info;
    (void)status_flags;

    struct AudioContext* ctx = (struct AudioContext*)userdata;
    float* stream = (float*)output;

    float t_per_frame = (1.0 / SAMPLE_RATE);

	for(int i = 0; i < VOICES_LEN; i++) {
    	if(ctx->voices[i].soundstack_len == 0) {
        	continue;
    	}
    	struct Sound* sound = &ctx->voices[i].soundstack[ctx->voices[i].soundstack_len - 1];
    	for(int j = 0; j < ctx->voices[i].soundstack_len - 1; j++) {
			ctx->voices[i].soundstack[j].t += t_per_frame * frames_per_buf;
    	}

    	if(sound->t > MAX_T_SECONDS) {
    		sound->t -= MAX_T_SECONDS;
    	}
	}

	for(uint32_t i = 0; i < frames_per_buf; i++) {
    	stream[i] = 0;

		for(uint8_t j = 0; j < VOICES_LEN; j++) {
        	if(ctx->voices[j].soundstack_len == 0) {
            	continue;
        	}
        	struct Sound* sound = &ctx->voices[j].soundstack[ctx->voices[j].soundstack_len - 1];

			if(!sound->active) {
    			continue;
			}
    		
    		sound->t += t_per_frame;

    		if((uint32_t)(sound->t * sound->freq) % 2 == 0) {
        		stream[i] += sound->amp;
    		} else {
        		stream[i] -= sound->amp;
    		}
    		//stream[i] += sin(sound->t * 3.14159 * sound->freq) * sound->amp;
		}

		if(stream[i] > 0.99) stream[i] = 0.99;
		if(stream[i] < -0.99) stream[i] = -0.99;
	}

	return 0;
}

void sound_play(struct AudioContext* ctx, struct Sound sound) {
    sound.amp = 0;
    sound.freq = 0;
    sound.t = 0;
    sound.active = true;

	// TODO: Better voice selection
    struct Voice* voice = &ctx->voices[0];
    if(sound.callback == snd_match) {
        voice = &ctx->voices[1];
    }

    voice->soundstack[voice->soundstack_len] = sound;
    voice->soundstack_len++;
}
