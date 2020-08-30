#include <memory.h>
#include <assert.h>
#include <stdint.h>
#include "chip8.h"

const uint8_t chip8_default_character_set[] = {
    0xf0, 0x90, 0x90, 0x90, 0xf0,
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xf0, 0x10, 0xf0, 0x80, 0xf0,
    0xf0, 0x10, 0xf0, 0x10, 0xf0,
    0x90, 0x90, 0xf0, 0x10, 0x10,
    0xf0, 0x80, 0xf0, 0x10, 0xf0,
    0xf0, 0x80, 0xf0, 0x90, 0xf0,
    0xf0, 0x10, 0x20, 0x40, 0x40,
    0xf0, 0x90, 0xf0, 0x90, 0xf0,
    0xf0, 0x90, 0xf0, 0x10, 0xf0,
    0xf0, 0x90, 0xf0, 0x90, 0x90,
    0xe0, 0x90, 0xe0, 0x90, 0xe0,
    0xf0, 0x80, 0x80, 0x80, 0xf0,
    0xe0, 0x90, 0x90, 0x90, 0xe0,
    0xf0, 0x80, 0xf0, 0x80, 0xf0,
    0xf0, 0x80, 0xf0, 0x80, 0x80};

void chip8_init(struct chip8 *cpu)
{
    memset(cpu, 0, sizeof(struct chip8));
    memcpy(&cpu->ram.memory_array, chip8_default_character_set, sizeof(chip8_default_character_set));
}
void chip8_exec(struct chip8 *cpu, uint16_t opcode)
{
    switch (opcode)
    {
    case 0x00E0:
        chip8_screen_clear(&cpu->screen);
        break;
    case 0x00EE:
        cpu->registers.PC = chip8_stack_pop(cpu);
        break;
        /*
    case 0x00E0:
        break;
    case 0x00E0:
        break;
    case 0x00E0:
        break;
    case 0x00E0:
        break;
    case 0x00E0:
        break;
    case 0x00E0:
        break;
    case 0x00E0:
        break;
    case 0x00E0:
        break;
    case 0x00E0:
        break;
    case 0x00E0:
        break;
    case 0x00E0:
        break;
    case 0x00E0:
        break;
    case 0x00E0:
        break;
    case 0x00E0:
        break;
    case 0x00E0:
        break;
    case 0x00E0:
        break;
    case 0x00E0:
        break;
    case 0x00E0:
        break;
    case 0x00E0:
        break;
    case 0x00E0:
        break;
    case 0x00E0:
        break;
    case 0x00E0:
        break;
    case 0x00E0:
        break;
    case 0x00E0:
        break;
    case 0x00E0:
        break;
    case 0x00E0:
        break;
    case 0x00E0:
        break;
    case 0x00E0:
        break;
    case 0x00E0:
        break;
    case 0x00E0:
        break;
    case 0x00E0:
        break;
        */
    }
}

void chip8_load(struct chip8 *cpu, const char *buf, size_t size)
{
    assert(size + PROGRAM_LOAD_ADDRESS < MEMORY_SIZE);
    memcpy(&cpu->ram.memory_array[PROGRAM_LOAD_ADDRESS], buf, size);
    cpu->registers.PC = PROGRAM_LOAD_ADDRESS;
}