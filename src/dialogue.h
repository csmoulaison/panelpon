#ifndef dialogue_h_INCLUDED
#define dialogue_h_INCLUDED

#include "stdint.h"

#include "levels.h"
#include "draw.h"

// TODO - Automate based on txt file
#define DIALOGUE_MAX_LEN 2048
#define SEQUENCE_LEN 8
#define SEQUENCE_PITCH 256

struct Dialogue {
	char string[DIALOGUE_MAX_LEN];
	uint32_t sequence_starts[LEVELS_LEN * SEQUENCE_LEN];
	uint16_t sequence;
	uint8_t sequence_char;
	uint8_t char_countdown;
};

void dialogue_loop(struct Dialogue* dialogue);
void dialogue_draw(struct Dialogue* dialogue, struct DrawContext* ctx);

#endif // dialogue_h_INCLUDED

