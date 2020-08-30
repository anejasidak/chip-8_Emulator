#include "chip8_screen.h"
#include <assert.h>

static void chip8_screen_check_bounds(int x, int y)
{
    assert(x >= 0 && x < DISPLAY_WIDTH && y >= 0 && y < DISPLAY_HEIGHT);
}
void chip8_screen_set(struct chip8_screen *screen, int x, int y)
{
    chip8_screen_check_bounds(x, y);
    screen->pixels[y][x] = true;
}
bool chip8_screen_is_set(struct chip8_screen *screen, int x, int y)
{
    chip8_screen_check_bounds(x, y);
    return screen->pixels[y][x];
}

bool chip8_screen_draw_sprite(struct chip8_screen *screen, int x, int y, const uint8_t *sprite, int num)
{
    bool pixel_collision = false;

    for (int ly = 0; ly < num; ly++)
    {
        uint8_t c = sprite[ly];
        for (int lx = 0; lx < 8; lx++)
        {
            if (c & (0b10000000 >> lx))
            {
                if (screen->pixels[(y + ly) % DISPLAY_HEIGHT][(x + lx) % DISPLAY_WIDTH])
                {
                    pixel_collision = true;
                }
                screen->pixels[(y + ly) % DISPLAY_HEIGHT][(x + lx) % DISPLAY_WIDTH] ^= true;
            }
        }
    }
    return pixel_collision;
}
