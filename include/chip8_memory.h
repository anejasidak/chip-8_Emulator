#ifndef CHIP8_MEMORY_H_
#define CHIP8_MEMORY_H_

#include <stdint.h>
#include "config.h"

struct chip8_memory
{
    uint8_t memory_array[MEMORY_SIZE];
};

void chip8_memory_set(struct chip8_memory *ram, int index, uint8_t value);
uint8_t chip8_memory_get(struct chip8_memory *ram, int index);


#endif