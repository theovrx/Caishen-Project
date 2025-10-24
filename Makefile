CC=gcc
CFLAGS=-Wall
LDFLAGS=-I/usr/include/SDL2 -D_REENTRANT -lSDL2 -lSDL2_ttf

test :
	${CC} font-test.c ${LDFLAGS}
