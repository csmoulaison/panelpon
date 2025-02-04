CFLAGS = -std=c17 -Wall -Wextra -g -I/extern
LDFLAGS = -lportaudio -lrt -lasound -ljack -pthread -lSDL2main -lSDL2 -ldl -lm -lpthread
EXE = bin/panelpon
SRC = $(wildcard src/*.c)

panelpon: $(SRC)
	gcc $(CFLAGS) -o $(EXE) $(SRC) $(LDFLAGS)
	sh build/generate_atlas.sh

.PHONY: test clean

test: panelpon
	$(EXE)

clean:
	rm -f $(EXE)
