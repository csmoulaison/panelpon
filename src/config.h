#ifndef config_h_INCLUDED
#define config_h_INCLUDED

// Time config
#define MS_PER_FRAME 8
#define DT MS_PER_FRAME / 1000.0

// Lengths config
#define FRAMES_YOFF 48
#define FRAMES_SHIFT 24
#define FRAMES_FALL 6
#define FRAMES_MATCH 125
#define FRAMES_HITCH 200
#define FRAMES_CURSOR 12
#define FRAMES_GRACE FRAMES_HITCH

// Screen config
#define LOGICAL_W 216
#define LOGICAL_H 216
// Tile size
#define TSIZE 8

// Match config
#define SHAPES_LEN 5
#define BOARD_W 6
#define BOARD_H 13
#define BOARD_LEN BOARD_W * BOARD_H

// Paths
#define PATH_ICON "icon.bmp"
#define PATH_ATLAS_PRIMARY "atlas_primary.bmp"
#define PATH_ATLAS_SECONDARY "atlas_secondary.bmp"

// Sprites
#define SPR_PIXEL (struct IRect){136, 0, 1, 1}
#define SPR_TILE_MOVE_OFFSET 16
#define SPR_TILE_MOVE_VERT_OFFSET 64
#define SPR_TILE_MOVE_WARP_OFFSET 112
#define SPR_TILE_MOVE_FRAMES 3
#define SPR_TILE_INACTIVE_OFFSET 8
#define SPR_TILE_MOVE_FRAME_LEN FRAMES_SHIFT / (SPR_TILE_MOVE_FRAMES * 2)
#define SPR_SQUARE   (struct IRect){0, 0, 8, 8}
#define SPR_DIAMOND  (struct IRect){0, 8, 8, 8}
#define SPR_ROUND    (struct IRect){0, 16, 8, 8}
#define SPR_CROSS    (struct IRect){0, 24, 8, 8}
#define SPR_TRIANGLE (struct IRect){0, 32, 8, 8}
#define SPR_CAPSULE  (struct IRect){0, 40, 8, 8}
#define SPR_CURSOR_SQUARE (struct IRect){136, 0, 8, 8}
#define SPR_CURSOR_HBIAS  (struct IRect){136, 8, 8, 8}
#define SPR_CURSOR_VBIAS  (struct IRect){136, 16, 8, 8}
// Cursor animation
#define SPR_CURSOR_MOVE_FRAMES 2
#define SPR_CURSOR_MOVE_FRAME_LEN FRAMES_CURSOR / SPR_CURSOR_MOVE_FRAMES
#define SPR_CURSOR_HDRAG_OFF 8
#define SPR_CURSOR_HPUSH_OFF 24
#define SPR_CURSOR_VDRAG_OFF 40
#define SPR_CURSOR_VPUSH_OFF 56
// Debug sprites
#define SPR_DEBUG_SHIFT (struct IRect){136,  40, 8, 8}
#define SPR_DEBUG_FALL  (struct IRect){144,  40, 8, 8}
#define SPR_DEBUG_MATCH (struct IRect){152,  40, 8, 8}
#define SPR_DEBUG_BUF   (struct IRect){160,  40, 8, 8}

#endif // config_h_INCLUDED
