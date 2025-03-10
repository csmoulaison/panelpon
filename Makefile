CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -g -I/extern
LDFLAGS = -lportaudio -lrt -lasound -ljack -pthread -lSDL2main -lSDL2 -ldl -lm -lpthread
EXE = bin/panelpon
SRC = $(wildcard src/*.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $^ $(LDFLAGS)
	sh build/generate_atlas.sh

.PHONY: clean

clean:
	rm -f $(EXE)
	rm -f src/*.o
