#ifndef CHIP8_H_
#define CHIP8_H_

#include "chip8_memory.h"
#include "chip8_registers.h"
#include "chip8_stack.h"
#include "chip8_keyboard.h"
#include "chip8_screen.h"
#include <stddef.h>

struct chip8
{
    struct chip8_memory ram;
    struct chip8_registers registers;
    struct chip8_stack stack;
    struct chip8_keyboard keyboard;
    struct chip8_screen screen;
    
};

void chip8_init(struct chip8 *cpu);
void chip8_exec(struct chip8 *cpu, uint16_t opcode);
void chip8_load(struct chip8* cpu, const char* buf, size_t size);

#endif