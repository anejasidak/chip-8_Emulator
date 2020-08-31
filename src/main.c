#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <unistd.h>

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
    if (argc < 2)
    {
        printf("Please provide a file to load\n");
        return -1;
    }

    const char *filename = argv[1];
    FILE *f = fopen(filename, "r");

    if (!f)
    {
        printf("Failed to open file\n");
        return -1;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char buf[size];

    if (fread(buf, size, 1, f) != 1)
    {
        printf("Could not read file\n");
        fclose(f);
        return -1;
    }
    fclose(f);

    struct chip8 cpu;
    chip8_init(&cpu);
    chip8_load(&cpu, buf, size);

    chip8_keyboard_set_map(&cpu.keyboard, keyboard_map);

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
                int vkey = chip8_keyboard_map(&cpu.keyboard, key);
                if (vkey != -1)
                {
                    chip8_keyboard_press_down(&cpu.keyboard, vkey);
                }
            }
            break;
            case SDL_KEYUP:
            {

                char key = event.key.keysym.sym;
                int vkey = chip8_keyboard_map(&cpu.keyboard, key);
                if (vkey != -1)
                {
                    chip8_keyboard_press_up(&cpu.keyboard, vkey);
                }
            }
            break;
            }
        }
        if (!isActive)
        {
            break;
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

        for (int x = 0; x < DISPLAY_WIDTH; x++)
        {
            for (int y = 0; y < DISPLAY_HEIGHT; y++)
            {
                if (chip8_screen_is_set(&cpu.screen, x, y))
                {
                    SDL_Rect r;
                    r.x = x * DISPLAY_SCALE;
                    r.y = y * DISPLAY_SCALE;
                    r.w = DISPLAY_SCALE;
                    r.h = DISPLAY_SCALE;
                    SDL_RenderFillRect(renderer, &r);
                }
            }
        }
        SDL_RenderPresent(renderer);
        if (cpu.registers.delay_timer > 0)
        {
            sleep(0.9);
            cpu.registers.delay_timer--;
        }
        if (cpu.registers.sound_timer > 0)
        {
            //No beeping sound implemented yet
            cpu.registers.sound_timer = 0;
        }

        
        uint16_t opcode = chip8_memory_get_ins(&cpu.ram, cpu.registers.PC);
        cpu.registers.PC += 2;
        chip8_exec(&cpu, opcode);
        printf("%02x\n", opcode);
        sleep(0.9);
    }

    // Close the window and quit SDL to  clean up all initialized subsystems.
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}