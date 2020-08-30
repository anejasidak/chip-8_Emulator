#include <stdint.h>
#include <assert.h>

#include "config.h"
#include "chip8_memory.h"

static void chip8_memory_is_in_bound(int index)
{
    assert(index >= 0 && index < MEMORY_SIZE);
}
void chip8_memory_set(struct chip8_memory *ram, int index, uint8_t value)
{
    chip8_memory_is_in_bound(index);
    ram->memory_array[index] = value;
}
uint8_t chip8_memory_get(struct chip8_memory *ram, int index)
{
    chip8_memory_is_in_bound(index);
    return ram->memory_array[index];
}

uint16_t chip8_memory_get_ins(struct chip8_memory *ram, int index)
{
    uint8_t byte1 = chip8_memory_get(ram, index);
    uint8_t byte2 = chip8_memory_get(ram, index+1);

    return byte1 << 8 | byte2;
}
