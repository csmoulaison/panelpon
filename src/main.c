#include "stdio.h"

#include "init.h"
#include "loop.h"

int main() {
    Context ctx;
    init(&ctx);

	while(!ctx.input.quit.held && !ctx.input.quit_event) {
    	loop(&ctx);
	}
    
    return 0;
}
