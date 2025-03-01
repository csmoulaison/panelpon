#include "cursor.h"

#include "board.h"

#define HFLIP SDL_FLIP_HORIZONTAL
#define VFLIP SDL_FLIP_VERTICAL

// Moves the cursor, clamping its coordinates between 0 and the given maximums.
bool cur_move_bounded(struct Cursor* cursor, bool up, bool down, bool left, bool right, uint8_t xmax, uint8_t ymax) {
	uint8_t curx = xcoord(cursor->pos);
	uint8_t cury = ycoord(cursor->pos);

   	if(up)    cury -= 1;
	if(down)  cury += 1;
	if(left)  curx -= 1;
	if(right) curx += 1;

	if(cury == 255) cury = 0;
	if(curx == 255) curx = 0;
	if(cury > ymax) cury = ymax;
	if(curx > xmax) curx = xmax; // -2 because cursor is 2x1

	uint8_t prev = cursor->pos;
	cursor->pos = bindex(curx, cury);	

	bool moved = prev != cursor->pos;
	if(moved) {
		cursor->prev = prev;
		cursor->anim = 0;
	}
	return moved;
}

void draw_cursor_box(struct DrawContext* ctx, struct Cursor* cursor, struct IRect spr, struct Pallete pl, uint8_t sizex, uint8_t sizey, uint8_t yoff) {
	sizex *= TSIZE;
	sizey *= TSIZE;

	uint8_t curx = xcoord(cursor->pos) * TSIZE;
	uint8_t cury = ycoord(cursor->pos) * TSIZE + TSIZE - yoff;

	if(cursor->anim == FRAMES_CURSOR) {
		// Top left, bottom left, top right, bottom right
		draw_sprite     (ctx, spr, curx - 1,                 cury - 1,                 pl);
		draw_sprite_flip(ctx, spr, curx - 1,                 cury + sizey - TSIZE + 1, pl, VFLIP);
		draw_sprite_flip(ctx, spr, curx + sizex - TSIZE + 1, cury - 1,                 pl, HFLIP);
		draw_sprite_flip(ctx, spr, curx + sizex - TSIZE + 1, cury + sizey - TSIZE + 1, pl, HFLIP | VFLIP);
		return;
	}

	// TODO - Pretty ugly. I need to think more about the commonalities between the
	// 4 directions and refactor. I suggest the shallow things first to hopefully
	// visually deobfuscate the deeper things.

	uint8_t frame_len = SPR_CURSOR_MOVE_FRAME_LEN;
	uint8_t frame = cursor->anim / frame_len;
	uint8_t dst_xoff = frame * spr.w;
	uint8_t drag_off = (uint8_t[2]){0, 6}[frame];

	// Moving horizontally
	{
		uint8_t ytop = cury - 1;
		uint8_t ybottom = cury + sizey - TSIZE + 1;
		
		// Moving right
		if(cursor->pos == xoffset(cursor->prev, 1)) {
			// Left: top then bottom - dragging
			{
				int8_t x = curx - TSIZE - 1 + drag_off;
				struct IRect s = spr;
				s.x += SPR_CURSOR_HDRAG_OFF + dst_xoff;
				draw_sprite     (ctx, s, x, ytop,    pl);
				draw_sprite_flip(ctx, s, x, ybottom, pl, VFLIP);
			}
			// Right: top then bottom - pushing
			{
				uint8_t x = curx + sizex - TSIZE + 1;
				struct IRect s = spr;
				s.x += SPR_CURSOR_HPUSH_OFF + dst_xoff;
				draw_sprite_flip(ctx, s, x, ytop,    pl, HFLIP);
				draw_sprite_flip(ctx, s, x, ybottom, pl, HFLIP | VFLIP);
			}
			return;
		}

		// Moving left
		if(cursor->pos == xoffset(cursor->prev, -1)) {
			// Right: top then bottom - dragging
			{
				uint8_t x = curx + sizex + 1 - drag_off;
				struct IRect s = spr;
				s.x += SPR_CURSOR_HDRAG_OFF + dst_xoff;
				draw_sprite_flip(ctx, s, x, ytop,    pl, HFLIP);
				draw_sprite_flip(ctx, s, x, ybottom, pl, HFLIP | VFLIP);
			}
			// Left: top then bottom - pushing
			{
				int8_t x = curx - 1;
				struct IRect s = spr;
				s.x += SPR_CURSOR_HPUSH_OFF + dst_xoff;
				draw_sprite     (ctx, s, x, ytop,    pl);
				draw_sprite_flip(ctx, s, x, ybottom, pl, VFLIP);
			}
			return;
		}
	}

	// Moving vertically
	{
		int8_t xleft = curx - 1;
		uint8_t xright = curx + sizex - TSIZE + 1;
		
		// Moving down
		if(cursor->pos == yoffset(cursor->prev, 1)) {
			// Top: left then right - dragging
			{
				uint8_t y = cury - TSIZE - 1 + drag_off;
				struct IRect s = spr;
				s.x += SPR_CURSOR_VDRAG_OFF + dst_xoff;
				draw_sprite     (ctx, s, xleft,  y, pl);
				draw_sprite_flip(ctx, s, xright, y, pl, HFLIP);
			}
			// Bottom: left then right - pushing
			{
				uint8_t y = cury + sizey - TSIZE + 1;
				struct IRect s = spr;
				s.x += SPR_CURSOR_VPUSH_OFF + dst_xoff;
				draw_sprite_flip(ctx, s, xleft,  y, pl, VFLIP);
				draw_sprite_flip(ctx, s, xright, y, pl, VFLIP | HFLIP);
			}
			return;
		}
		// Moving up
		if(cursor->pos == yoffset(cursor->prev, -1)) {
			// Bottom: left then right - dragging
			{
				uint8_t y = cury + sizey + 1 - drag_off;
				struct IRect s = spr;
				s.x += SPR_CURSOR_VDRAG_OFF + dst_xoff;
				draw_sprite_flip(ctx, s, xleft,  y, pl, VFLIP);
				draw_sprite_flip(ctx, s, xright, y, pl, VFLIP | HFLIP);
			}
			// Top: left then right - pushing
			{
				uint8_t y = cury - 1;
				struct IRect s = spr;
				s.x += SPR_CURSOR_VPUSH_OFF + dst_xoff;
				draw_sprite     (ctx, s, xleft,  y, pl);
				draw_sprite_flip(ctx, s, xright, y, pl, HFLIP);
			}
			return;
		}
	}
}
