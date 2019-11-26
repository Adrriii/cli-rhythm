FILESSRC 	= $(wildcard src/*.c)
OBJSRC		= $(FILESSRC:src/%.c=build/%.o)
CC		= gcc
CFLAGS=-I.

all: $(OBJSRC)
	$(CC) $(CFLAGS) $(OBJSRC) -o cli-rhythm

build/%.o: src/%.c
	mkdir -p build
	$(CC) -c $(CFLAGS) $^ -o $@
