CFLAGS = -std=c17
LDFLAGS = -lSDL2main -lSDL2 
EXE = bin/panelpon
SRC = $(wildcard src/*.c)

panelpon: $(SRC)
	gcc $(CFLAGS) -o $(EXE) $(SRC) $(LDFLAGS)

.PHONY: test clean

test: panelpon
	$(EXE)

clean:
	rm -f $(EXE)
