#ifndef config_h_INCLUDED
#define config_h_INCLUDED

// Time config
#define MS_PER_FRAME 8
#define DT MS_PER_FRAME / 1000.0

// Lengths config
#define FRAMES_YOFF 32
#define FRAMES_FLIP 24
// actually 6
#define FRAMES_FALL 6
#define FRAMES_EXPLODE 125
#define FRAMES_HITCH 125
#define FRAMES_CURSOR 12

// Screen config
#define LOGICAL_W 216
#define LOGICAL_H 216

// Match config
#define SHAPES_LEN 4
#define BOARD_W 8
#define BOARD_H 9
#define BOARD_LEN BOARD_W * BOARD_H

// Paths
#define PATH_ICON "icon.bmp"
#define PATH_ATLAS_PRIMARY "atlas_primary.bmp"
#define PATH_ATLAS_SECONDARY "atlas_secondary.bmp"

// Sprites
#define SPR_PIXEL (struct IRect){56, 0, 1, 1}
#define SPR_TILE_MOVE_OFFSET 8
#define SPR_TILE_MOVE_FRAMES 3
#define SPR_TILE_MOVE_FRAME_LEN FRAMES_FLIP / (SPR_TILE_MOVE_FRAMES * 2)
#define SPR_SQUARE   (struct IRect){0, 0, 8, 8}
#define SPR_DIAMOND  (struct IRect){0, 8, 8, 8}
#define SPR_ROUND    (struct IRect){0, 16, 8, 8}
#define SPR_CROSS    (struct IRect){0, 24, 8, 8}
#define SPR_TRIANGLE (struct IRect){0, 32, 8, 8}
#define SPR_CAPSULE  (struct IRect){0, 40, 8, 8}
#define SPR_CURSOR_SQUARE (struct IRect){56, 0, 8, 8}
#define SPR_CURSOR_HBIAS  (struct IRect){56, 8, 8, 8}
#define SPR_CURSOR_VBIAS  (struct IRect){56, 16, 8, 8}
// Cursor animation
#define SPR_CURSOR_MOVE_FRAMES 2
#define SPR_CURSOR_MOVE_FRAME_LEN FRAMES_CURSOR / SPR_CURSOR_MOVE_FRAMES
// #define SPR_CURSOR_MOVE_R  (struct IRect){18, 48, 27, 10}
// #define SPR_CURSOR_MOVE_D  (struct IRect){0, 58, 18, 19}
// Debug sprites
#define SPR_DEBUG_FLIP (struct IRect){0, 96, 8, 8}
#define SPR_DEBUG_FALL (struct IRect){8, 96, 8, 8}
#define SPR_DEBUG_EXPLODE (struct IRect){16, 96, 8, 8}
#define SPR_DEBUG_BUF (struct IRect){24, 96, 8, 8}

#endif // config_h_INCLUDED
