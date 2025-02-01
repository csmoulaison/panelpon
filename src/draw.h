#ifndef draw_h_INCLUDED
#define draw_h_INCLUDED

#include "SDL2/SDL.h"
#include "stdint.h"

#include "context.h"
#include "palletes.h"
#include "rect.h"

void draw_clear(Context* ctx);
void draw_present(Context* ctx);
void draw_sprite(Context* ctx, IRect src, IRect dst, Pallete pallete);

#endif // draw_h_INCLUDED
