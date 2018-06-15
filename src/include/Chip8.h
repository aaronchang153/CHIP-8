#ifndef CHIP8_H
#define CHIP8_H

#include <stdlib.h>
#include <string.h>

#define VX(n)       (0x0F00 & n)>>8
#define VY(n)       (0x00F0 & n)>>4
#define IMM4(n)     (0x000F & n)
#define IMM8(n)     (0x00FF & n)
#define IMM12(n)    (0x0FFF & n)

#define MEMORY_SIZE 4096


typedef struct{
    unsigned short opcode;
    unsigned char V[16];
    unsigned short pc;
    unsigned short I;
    unsigned short sp;
    unsigned short stack[16];
    unsigned char memory[MEMORY_SIZE];
    unsigned char delay_timer;
    unsigned char sound_timer;
    unsigned char gfx[64 * 32];
} Chip8;

Chip8 *Chip8_Create();

void Chip8_Destroy(Chip8 *c);

void Chip8_Init(Chip8 *c);


#endif // CHIP8_H