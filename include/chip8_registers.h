#ifndef CHIP8_REGISTERS_H_
#define CHIP8_REGISTERS_H_

#include "config.h"
#include <stdint.h>

struct chip8_registers
{
    uint8_t V[TOTAL_DATA_REGISTERS];

    uint16_t I;
    uint16_t delay_timer;
    uint16_t sound_timer;
    uint16_t PC;
    uint8_t SP;

};
#endif