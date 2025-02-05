#ifndef config_h_INCLUDED
#define config_h_INCLUDED

// Screen config
#define LOGICAL_W 216
#define LOGICAL_H 216

// Match config
#define SHAPES_LEN 4
#define BOARD_W 8
#define BOARD_H 17
#define BOARD_LEN BOARD_W * BOARD_H
#define TIME_EXPLODE 1

// Paths
#define PATH_ICON "icon.bmp"
#define PATH_ATLAS_PRIMARY "atlas_primary.bmp"
#define PATH_ATLAS_SECONDARY "atlas_secondary.bmp"

// Sprites
#define SPR_PIXEL (struct IRect){0, 32, 1, 1}
#define SPR_TILE_MOVE_OFFSET 8 // Start of tile movement animation as offset from the tile sprite
#define SPR_TILE_MOVE_FRAMES 3
#define SPR_SQUARE  (struct IRect){0, 0, 8, 8}
#define SPR_DIAMOND (struct IRect){0, 8, 8, 8}
#define SPR_ROUND   (struct IRect){0, 16, 8, 8}
#define SPR_CROSS   (struct IRect){0, 24, 8, 8}
#define SPR_CURSOR  (struct IRect){0, 32, 18, 10}
#define SPR_CURSOR_MOVE_FRAMES 2
#define SPR_CURSOR_MOVE_R  (struct IRect){18, 32, 27, 10}
#define SPR_CURSOR_MOVE_D  (struct IRect){0, 42, 18, 19}

#endif // config_h_INCLUDED
