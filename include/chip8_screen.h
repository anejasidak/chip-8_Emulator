#ifndef CHIP8_SCREEN_H_
#define CHIP8_SCREEN_H_

#include <stdbool.h>
#include <stdint.h>
#include "config.h"

struct chip8_screen 
{
    bool pixels[DISPLAY_HEIGHT][DISPLAY_WIDTH];
};

void chip8_screen_set(struct chip8_screen* screen, int x, int y);
bool chip8_screen_is_set(struct chip8_screen* screen, int x, int y);
bool chip8_screen_draw_sprite (struct chip8_screen * screen, int x, int y, const uint8_t* sprite, int num);

#endif