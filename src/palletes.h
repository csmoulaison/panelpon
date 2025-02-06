#ifndef palletes_h_INCLUDED
#define palletes_h_INCLUDED

#include "stdint.h"

#define PL_ALL_WHITE (struct Pallete){0xFFFFFF, 0xFFFFFF}
#define PL_ALL_BLACK (struct Pallete){0x000000, 0x000000}
#define PL_BLUE      (struct Pallete){0x0000FF, 0xFFFFFF}
#define PL_RED 		 (struct Pallete){0xFF0000, 0xFFFFFF}
#define PL_GREEN     (struct Pallete){0x00FF00, 0xFFFFFF}
#define PL_PURPLE    (struct Pallete){0xFF00FF, 0xFFFFFF}
#define PL_YELLOW    (struct Pallete){0xFFFF00, 0xFFFFFF}
#define PL_CYAN      (struct Pallete){0x00FFFF, 0xFFFFFF}

struct Pallete {
    uint32_t primary;
    uint32_t secondary;
};

#endif // palletes_h_INCLUDED
