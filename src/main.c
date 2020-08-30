#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

#include "config.h"
#include "chip8.h"
#include "chip8_memory.h"

static char keyboard_map[TOTAL_KEYS] = {
    SDLK_0, SDLK_1, SDLK_2, SDLK_3,
    SDLK_4, SDLK_5, SDLK_6, SDLK_7,
    SDLK_8, SDLK_9, SDLK_a, SDLK_b,
    SDLK_c, SDLK_d, SDLK_e, SDLK_f};

int main(int argc, char **argv)
{
    struct chip8 cpu;
    chip8_init(&cpu);

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
        DISPLAY_WIDTH * DISPLAY_SCALE, DISPLAY_HEIGHT * DISPLAY_SCALE, 0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);

    bool isActive = true;
    while (isActive)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
            {
                isActive = false;
            }
            break;

            case SDL_KEYDOWN:
            {
                char key = event.key.keysym.sym;
                int vkey = chip8_keyboard_map(keyboard_map, key);
                if (vkey != -1)
                {
                    chip8_keyboard_press_down(&cpu.keyboard, vkey);
                }
            }
            break;
            case SDL_KEYUP:
            {

                char key = event.key.keysym.sym;
                int vkey = chip8_keyboard_map(keyboard_map, key);
                if (vkey != -1)
                {
                    chip8_keyboard_press_up(&cpu.keyboard, vkey);
                }
            }
            break;
            }
        }
        if (isActive)
        {
            SDL_RenderClear(renderer);
        }
    }

    // Close the window and quit SDL to  clean up all initialized subsystems.
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}