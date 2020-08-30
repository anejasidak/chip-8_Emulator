FLAGS = -Wall -Werror -g
SDL = -lSDL2 `sdl2-config --libs --cflags`
FILES = ./src/main.c

EXECUTABLE = emulator
all: 
	gcc ${FLAGS} ${SDL} ${FILES} -o ./bin/${EXECUTABLE}