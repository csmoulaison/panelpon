#ifndef transition_h_INCLUDED
#define transition_h_INCLUDED

#include "context.h"

void fade_transition(struct Context* ctx, void(*callback)(struct Context* ctx));
struct Pallete pl_from_transition_state(struct Pallete pl, uint8_t state);

#endif // transition_h_INCLUDED
