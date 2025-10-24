CC=gcc
CFLAGS=-Wall
LDFLAGS=-I/usr/include/SDL2 -D_REENTRANT -lSDL2 -lSDL2_ttf -lSDL2_image

test :
	${CC} font-test.c ${LDFLAGS}

main :
	${CC} main.c ${LDFLAGS}

