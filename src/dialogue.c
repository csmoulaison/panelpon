#include "dialogue.h"

#define DIALOGUE_W 128
#define DIALOGUE_H 64

void dialogue_loop(struct Dialogue* dialogue) {
	if(dialogue->string[dialogue->sequence * SEQUENCE_PITCH + dialogue->sequence_char] != ';') {
		dialogue->char_countdown--;
		if(dialogue->char_countdown == 0) {
			dialogue->char_countdown = FRAMES_DIALOGUE_CHAR;
			dialogue->sequence_char++;
		}
	}
}

void dialogue_draw(struct Dialogue* dialogue, struct DrawContext* ctx) {
	ctx->xoff = LOGICAL_W / 2 - DIALOGUE_W / 2;
	ctx->yoff = LOGICAL_H / 2 - DIALOGUE_H / 2;

	draw_rect(ctx, (struct IRect){0, 0, DIALOGUE_W, DIALOGUE_H}, PL_ALL_WHITE);

	uint32_t start = dialogue->sequence * SEQUENCE_PITCH;
	for(uint32_t i = start; i < start + dialogue->sequence_char; i++) {
	}
}
