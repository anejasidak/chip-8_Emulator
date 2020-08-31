#include <assert.h>
#include "chip8_keyboard.h"

static void chip8_keyboard_is_in_bound(int key)
{
    assert(key >= 0 && key < TOTAL_KEYS);
}

void chip8_keyboard_set_map(struct chip8_keyboard *keyboard, const char* map) {
    keyboard->keyboard_map = map;
}
int chip8_keyboard_map(struct chip8_keyboard *keyboard, char key)
{
    for (int i = 0; i < TOTAL_KEYS; i++)
    {
        if (keyboard->keyboard_map[i] == key)
        {
            return i;
        }
    }
    return -1;
}
void chip8_keyboard_press_down(struct chip8_keyboard *keyboard, int key)
{
    chip8_keyboard_is_in_bound(key);
    keyboard->keyboad[key] = true;
}
void chip8_keyboard_press_up(struct chip8_keyboard *keyboard, int key)
{

    chip8_keyboard_is_in_bound(key);
    keyboard->keyboad[key] = false;
}
bool chip8_keyboard_is_down(struct chip8_keyboard *keyboard, int key)
{

    chip8_keyboard_is_in_bound(key);
    return keyboard->keyboad[key];
}
