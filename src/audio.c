#include "audio.h"

void audio_data_callback(ma_device* device, void* output, const void* input, uint32_t frames_len) {
    ma_waveform* wave = &((AudioCallbackData*)device->pUserData)->wave;
    ma_waveform_read_pcm_frames(wave, output, frames_len, NULL);
}
