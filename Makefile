CC      := cc
CFLAGS  := -Wall -Wextra -Wpedantic -std=c11 -g -Iinclude
SRC     := $(wildcard src/*.c)
HDR     := $(wildcard include/*.h)
OBJ     := $(SRC:src/%.c=out/%.o)
BIN     := out/pstk

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $@

out/%.o: src/%.c $(HDR) | out
	$(CC) $(CFLAGS) -c $< -o $@

out:
	mkdir -p out

run: $(BIN)
	./$(BIN)

clean:
	rm -rf out

.PHONY: all run clean
