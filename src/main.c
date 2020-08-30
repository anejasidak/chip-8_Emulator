#include <stdio.h>
#include <SDL2/SDL.h>

#include "config.h"
#include "chip8.h"
#include "chip8_memory.h"

int main(int argc, char **argv)
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Unable to initialize SDL: %s", SDL_GetError());
        return -1;
    }

    /* Create a window for the emulator.
    *  Note: 
    *       The origin chip-8 display is 64 by 32 pixels. 
    *       To have better viewing, the display has been scaled up by 10 times.
    */
    SDL_Window *window = SDL_CreateWindow(
        EMULATOR_WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        DISPLAY_WIDTH * DISPLAY_SCALE, DISPLAY_HEIGHT * DISPLAY_SCALE, 0
    );


    struct chip8 cpu;
    memset(&cpu, 0, sizeof cpu);
    memory_set(&cpu.ram, 50, 100);
    printf("%d\n", memory_get(&cpu.ram, 50));










    while (1) // Conditions added later
    {
    }

    // Close the window and quit SDL to  clean up all initialized subsystems.
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}