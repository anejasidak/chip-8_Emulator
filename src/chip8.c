#include <memory.h>
#include <assert.h>
#include <stdint.h>
#include "chip8.h"

static void chip8_exec_extended(struct chip8 *cpu, uint16_t opcode);

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

void chip8_load(struct chip8 *cpu, const char *buf, size_t size)
{
    assert(size + PROGRAM_LOAD_ADDRESS < MEMORY_SIZE);
    memcpy(&cpu->ram.memory_array[PROGRAM_LOAD_ADDRESS], buf, size);
    cpu->registers.PC = PROGRAM_LOAD_ADDRESS;
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
    default:
        chip8_exec_extended(cpu, opcode);
        break;
    }
}

static void chip8_exec_extended(struct chip8 *cpu, uint16_t opcode)
{
    uint16_t *PC = &cpu->registers.PC;

    // Instruction format:
    // nnn or addr - A 12-bit value, the lowest 12 bits of the instruction
    // n or nibble - A 4-bit value, the lowest 4 bits of the instruction
    // x - A 4-bit value, the lower 4 bits of the high byte of the instruction
    // y - A 4-bit value, the upper 4 bits of the low byte of the instruction
    // kk or byte - An 8-bit value, the lowest 8 bits of the instruction

    uint16_t nnn = (opcode & 0x0fff);
    //uint8_t n = (opcode & 0x000f);
    uint8_t x = (opcode & 0x0f00) >> 8;
    uint8_t y = (opcode & 0x00f0) >> 4;
    uint8_t kk = (opcode & 0x00ff);

    switch (opcode & 0xf000)
    {
    case 0x1000:
        // 1nnn - Jp addr, Jump to location nnn.
        *PC = nnn;
        break;
    case 0x2000:
        // 2nnn - CALL addr, Call subroutine at nnn.
        chip8_stack_push(cpu, *PC);
        *PC = nnn;
        break;
    case 0x3000:
        // 3xkk - SE Vx, byte, Skip next instruction if Vx = kk.
        if (cpu->registers.V[x] == kk)
        {
            *PC = *PC + 2;
        }
        break;
    case 0x400:
        // 4xkk - SNE Vx, byte, Skip next instruction if Vx != kk.
        if (cpu->registers.V[x] != kk)
        {
            *PC = *PC + 2;
        }
        break;
    case 0x500:
        // 5xy0 - SE Vx, Vy, Skip next instruction if Vx = Vy.
        if (cpu->registers.V[x] == cpu->registers.V[x])
        {
            *PC = *PC + 2;
        }
        break;
    case 0x600:
        // 6xkk - LD Vx, byte, Set Vx = kk.
        cpu->registers.V[x] = kk;
        break;

    case 0x700:
        // 7xkk - ADD Vx, byte, Set Vx = Vx + kk.
        cpu->registers.V[x] += kk;
        break;
    }
}
