#ifndef CHIP8_MEMORY_H_
#define CHIP8_MEMORY_H_

struct chip8_memory
{
    uint8_t memory_array[MEMORY_SIZE];
};

void memory_set(struct chip8_memory *ram, int index, uint8_t value);
uint8_t memory_get(struct chip8_memory *ram, int index);


#endif