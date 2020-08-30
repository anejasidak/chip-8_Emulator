#include <memory.h>

#include "chip8.h"


void chip8_init(struct chip8 *cpu)
{
    memset(cpu, 0, sizeof (struct chip8));
}