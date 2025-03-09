#include "stdio.h"
#include "init.h"
#include "loop.h"
#include "destroy.h"

// TODO Cursors to add:
// * Layered, swap - 3d? - time travel?
// TODO - Menu with cursor + settings configuration.
// TODO - Bonus points.
// * Multiplier for chains.
// * Extra points for combos.

int main() {
	struct Context ctx;
	init(&ctx);

	while(!ctx.input.quit_event && ctx.prog_state != PROG_EXIT) {
    	loop(&ctx);
	}

	destroy(&ctx);
	return 0;
}
