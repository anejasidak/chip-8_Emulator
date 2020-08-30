#ifndef CHIP8_H_
#define CHIP8_H_

#include "chip8_memory.h"
#include "chip8_registers.h"
#include "chip8_stack.h"

struct chip8
{
    struct chip8_memory ram;
    struct chip8_registers registers;
    struct chip8_stack stack;
};

#endif