#include "stdio.h"
#include "init.h"
#include "loop.h"
#include "destroy.h"

// Cursors to add:
// * Tetris
// * Ring
// * Layered, swap - 3d? - time travel?

int main() {
	struct Context ctx;
	init(&ctx);

	while(!ctx.input.quit.held && !ctx.input.quit_event) {
    	loop(&ctx);
	}

	destroy(&ctx);
	return 0;
}
