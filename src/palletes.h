#ifndef palletes_h_INCLUDED
#define palletes_h_INCLUDED

#include "stdint.h"

#define PL_ALL_WHITE (Pallete){0xFFFFFF, 0xFFFFFF}
#define PL_BLUE      (Pallete){0x0000FF, 0xFFFFFF}
#define PL_RED 		 (Pallete){0xFF0000, 0xFFFFFF}
#define PL_GREEN     (Pallete){0x00FF00, 0xFFFFFF}
#define PL_PURPLE    (Pallete){0xFF00FF, 0xFFFFFF}

struct {
    uint32_t primary;
    uint32_t secondary;
} typedef Pallete;

#endif // palletes_h_INCLUDED
