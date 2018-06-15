#include "Chip8.h"

typedef void (*Chip8_Opcode)(Chip8 *c);

Chip8_Opcode chip8_table[16] = 
    {
        &Chip8_Decode_0xxx, &Chip8_Decode_1xxx, &Chip8_Decode_2xxx, &Chip8_Decode_3xxx,
        &Chip8_Decode_4xxx, &Chip8_Decode_5xxx, &Chip8_Decode_6xxx, &Chip8_Decode_7xxx,
        &Chip8_Decode_8xxx, &Chip8_Decode_9xxx, &Chip8_Decode_Axxx, &Chip8_Decode_Bxxx,
        &Chip8_Decode_Cxxx, &Chip8_Decode_Dxxx, &Chip8_Decode_Exxx, &Chip8_Decode_Fxxx
    };

unsigned char chip8_fontset[80] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

Chip8 *Chip8_Create(){
    Chip8 *chip8 = NULL;
    chip8 = (Chip8 *) malloc(sizeof(Chip8));
    Chip8_Init(chip8);
    return chip8;
}

void Chip8_Destroy(Chip8 *c){
    if(c != NULL){
        free(c);
    }
}

void Chip8_Init(Chip8 *c){
    if(c != NULL){
        memset((void *) c, 0, sizeof(Chip8));
        c->pc = 0x200;
        int i;
        for(i = 0; i < 80; i++){
            c->memory[i] = chip8_fontset[i];
        }
    }
    srand(time(NULL));
}

void Chip8_EmulateCycle(Chip8 *c){
    // Fetch
    c->opcode = (c->memory[c->pc] << 8) | c->memory[c->pc + 1];
    
    // Decode and execute
    chip8_table[(c->opcode & 0xF000) >> 12](c);
}