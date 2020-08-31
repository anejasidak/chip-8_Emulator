#include <memory.h>
#include <assert.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
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

static void chip8_exec_extended_eight(struct chip8 *cpu, uint16_t opcode)
{

    uint8_t x = (opcode & 0x0f00) >> 8;
    uint8_t y = (opcode & 0x00f0) >> 4;

    switch (opcode & 0x000f)
    {
    case 0x0:
        // 8xy0 - LD Vx, Vy, Set Vx = Vy.
        cpu->registers.V[x] = cpu->registers.V[y];
        break;

    case 0x1:
        // 8xy1 - OR Vx, Vy, Set Vx = Vx OR Vy.
        cpu->registers.V[x] |= cpu->registers.V[y];
        break;

    case 0x2:
        // 8xy2 - AND Vx, Vy, Set Vx = Vx AND Vy.
        cpu->registers.V[x] &= cpu->registers.V[y];
        break;
    case 0x3:
        // 8xy2 - XOR Vx, Vy, Set Vx = Vx XOR Vy.
        cpu->registers.V[x] ^= cpu->registers.V[y];
        break;
    case 0x4:
        //  8xy4 - ADD Vx, Vy, Set Vx = Vx + Vy, set VF = carry.
        cpu->registers.V[0x0f] = 0;
        uint16_t sum = cpu->registers.V[x] + cpu->registers.V[y];

        if (sum > 0xff)
        {
            cpu->registers.V[0x0f] = 1;
        }
        cpu->registers.V[x] = sum & 0x00ff;
        break;
    case 0x05:
        //  8xy5 - SUB Vx, Vy, Set Vx = Vx - Vy, set VF = NOT borrow.
        cpu->registers.V[0x0f] = 0;
        if (cpu->registers.V[x] > cpu->registers.V[y])
        {
            cpu->registers.V[0x0f] = 1;
        }
        cpu->registers.V[x] -= cpu->registers.V[y];
        break;
    case 0x06:
        // 8xy6 - SHR Vx {, Vy} Set Vx = Vx SHR 1.
        cpu->registers.V[0x0f] = cpu->registers.V[x] & 0x01;
        cpu->registers.V[x] >>= 2;
        break;
    case 0x07:
        // 8xy7 - SUBN Vx, Vy, Set Vx = Vy - Vx, set VF = NOT borrow.
        cpu->registers.V[0x0f] = cpu->registers.V[y] > cpu->registers.V[x];
        cpu->registers.V[y] -= cpu->registers.V[x];
        break;
    case 0x0E:
        cpu->registers.V[0x0f] = cpu->registers.V[x] & 0x80;
        cpu->registers.V[x] <<= 2;
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
    case 0x4000:
        // 4xkk - SNE Vx, byte, Skip next instruction if Vx != kk.
        if (cpu->registers.V[x] != kk)
        {
            *PC = *PC + 2;
        }
        break;
    case 0x5000:
        // 5xy0 - SE Vx, Vy, Skip next instruction if Vx = Vy.
        if (cpu->registers.V[x] == cpu->registers.V[y])
        {
            *PC = *PC + 2;
        }
        break;
    case 0x6000:
        // 6xkk - LD Vx, byte, Set Vx = kk.
        cpu->registers.V[x] = kk;
        break;

    case 0x7000:
        // 7xkk - ADD Vx, byte, Set Vx = Vx + kk.
        cpu->registers.V[x] += kk;
        break;
    case 0x8000:
        chip8_exec_extended_eight(cpu, opcode);
        break;
    case 0x9000:
        // 9xy0 - SNE Vx, Vy, Skip next instruction if Vx != Vy.
        if (cpu->registers.V[x] != cpu->registers.V[y])
        {
            *PC = *PC + 2;
        }
        break;
    case 0xA000:
        // Annn - LD I, addr, Set I = nnn.
        cpu->registers.I = nnn;
        break;
    case 0xB000:
        // Bnnn - JP V0, addr, Jump to location nnn + V0.
        *PC = *PC + cpu->registers.V[0x00] + nnn;
        break;
    case 0xC000:
        // Cxkk - RND Vx, byte, Set Vx = random byte AND kk.
        srand(clock());
        cpu->registers.V[x] = (rand() % 256) & kk;  
        break;
    }
}
