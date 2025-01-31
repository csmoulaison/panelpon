#ifndef palletes_h_INCLUDED
#define palletes_h_INCLUDED

#include "stdint.h"

#define PL_ALL_WHITE (Pallete){0xFFFFFFFFu, 0xFFFFFFFFu}
#define PL_BLUE      (Pallete){0x0000FFFFu, 0xFFFFFFFFu}
#define PL_RED 		 (Pallete){0xFF0000FFu, 0xFFFFFFFFu}
#define PL_GREEN     (Pallete){0x00FF00FFu, 0xFFFFFFFFu}
#define PL_PURPLE    (Pallete){0xFF00FFFFu, 0xFFFFFFFFu}

struct {
    uint32_t primary;
    uint32_t secondary;
} typedef Pallete;

#endif // palletes_h_INCLUDED
