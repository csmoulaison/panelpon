#include "destroy.h"

void destroy(Context* ctx) {
	ma_device_uninit(&ctx->audio.device);
}
