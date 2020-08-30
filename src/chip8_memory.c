#include <stdint.h>
#include <assert.h>

#include "config.h"
#include "chip8_memory.h"


static void memory_is_in_bound(int index)
{
    assert(index >= 0 && index < MEMORY_SIZE);
}
void memory_set(struct chip8_memory *ram, int index, uint8_t value)
{
    memory_is_in_bound(index);
    ram->memory_array[index] = value;
}
uint8_t memory_get(struct chip8_memory *ram, int index)
{
    memory_is_in_bound(index);
    return ram->memory_array[index];
}
