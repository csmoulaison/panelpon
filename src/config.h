#ifndef config_h_INCLUDED
#define config_h_INCLUDED

// Screen config
#define LOGICAL_W 216
#define LOGICAL_H 216

// Match config
#define SHAPES_LEN 5
#define BOARD_W 8
#define BOARD_H 17
#define BOARD_LEN BOARD_W * BOARD_H

// Paths
#define PATH_ICON "icon.bmp"
#define PATH_ATLAS_PRIMARY "atlas_primary.bmp"
#define PATH_ATLAS_SECONDARY "atlas_secondary.bmp"

// Sprites
#define SPR_PIXEL (struct IRect){0, 48, 1, 1}
#define SPR_TILE_MOVE_OFFSET 8 // Start of tile movement animation as offset from the tile sprite
#define SPR_TILE_MOVE_FRAMES 3
#define SPR_SQUARE   (struct IRect){0, 0, 8, 8}
#define SPR_DIAMOND  (struct IRect){0, 8, 8, 8}
#define SPR_ROUND    (struct IRect){0, 16, 8, 8}
#define SPR_CROSS    (struct IRect){0, 24, 8, 8}
#define SPR_TRIANGLE (struct IRect){0, 32, 8, 8}
#define SPR_CAPSULE  (struct IRect){0, 40, 8, 8}
#define SPR_CURSOR   (struct IRect){0, 48, 18, 10}
#define SPR_CURSOR_MOVE_FRAMES 2
#define SPR_CURSOR_MOVE_R  (struct IRect){18, 48, 27, 10}
#define SPR_CURSOR_MOVE_D  (struct IRect){0, 58, 18, 19}
// Debug sprites
#define SPR_DEBUG_1 (struct IRect){0, 80, 8, 8}
#define SPR_DEBUG_2 (struct IRect){8, 80, 8, 8}
#define SPR_DEBUG_3 (struct IRect){16, 80, 8, 8}

#endif // config_h_INCLUDED
