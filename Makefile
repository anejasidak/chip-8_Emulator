FLAGS = -Wall -Werror -g
SDL = -lSDL2 `sdl2-config --libs --cflags`
INCLUDE = -I ./include
FILES = ./src/*

EXECUTABLE = emulator
all: 
	gcc ${FLAGS} ${INCLUDE} ${SDL} ${FILES} -o ./bin/${EXECUTABLE}