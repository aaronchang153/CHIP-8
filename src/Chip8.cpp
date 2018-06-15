#include "Chip8.h"


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
}