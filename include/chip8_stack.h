#ifndef CHIP8_STACK_H_
#define CHIP8_STACK_H_

#include "config.h"
#include <stdint.h>

struct chip8;
struct chip8_stack
{
    uint16_t stack[TOTAL_STACK_DEPTH];
};

void chip8_stack_push(struct chip8 *cpu, uint16_t value);
uint16_t chip8_stack_pop(struct chip8 *cpu);
#endif