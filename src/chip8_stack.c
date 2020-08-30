#include <assert.h>
#include <stdint.h>

#include "chip8_stack.h"
#include "chip8.h"

static void  chip8_stack_in_bounds(struct chip8* cpu) {
    assert(cpu->registers.SP < TOTAL_STACK_DEPTH); 
}

void chip8_stack_push(struct chip8 *cpu, uint16_t value) {
    chip8_stack_in_bounds(cpu);
    
    cpu->stack.stack[cpu->registers.SP++] = value;
}
uint16_t chip8_stack_pop(struct chip8 *cpu) {

    cpu->registers.SP--;
    chip8_stack_in_bounds(cpu);

    return cpu->stack.stack[cpu->registers.SP];
}