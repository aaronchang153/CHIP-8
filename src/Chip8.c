#include "Chip8.h"

typedef void (*Chip8_Decode)(Chip8 *c);

Chip8_Decode chip8_table[16] = 
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

char Chip8_Load_Application(Chip8 *c, char *name){
    FILE *fp = fopen(name, "rb");
    if(fp == NULL){
        return 1;
    }
    else{
        unsigned char buffer;
        unsigned int read = 0;
        while(fread((void *) &buffer, 1, 1, fp)){
            c->memory[0x0200 + read] = buffer;
            read++;
        }
        return 0;
    }
}

void Chip8_EmulateCycle(Chip8 *c){
    // Fetch
    c->opcode = (c->memory[c->pc] << 8) | c->memory[c->pc + 1];
    
    // Decode and execute
    chip8_table[(c->opcode & 0xF000) >> 12](c);
    
    // pc increment varies based on the last instruction

    if(c->delay_timer > 0){
        c->delay_timer--;
    }
    if(c->sound_timer > 0){
        if(c->sound_timer == 1){
            printf("BEEP!\n");
        }
        c->sound_timer--;
    }
}

void Chip8_Decode_0xxx(Chip8 *c){
    switch((c->opcode & 0x000F)){
        case 0x0: // 0x00E0: Clear Screen
            memset((void *) c->gfx, 0, sizeof(char) * 64 * 32);
            c->pc += 2;
            break;
        case 0xE: // 0x00EE: Return from subroutine
            c->pc = c->stack[c->sp--];
            break;
    }
}

void Chip8_Decode_1xxx(Chip8 *c){
    // 0x1NNN: Jump
    c->pc = IMM12(c->opcode);
}

void Chip8_Decode_2xxx(Chip8 *c){
    // 0x2NNN: Jump SubRoutine
    c->stack[++c->sp] = c->pc + 2; // The pc increment may or may not need to 
                                   // go elsewhere
    c->pc = IMM12(c->opcode);
}

void Chip8_Decode_3xxx(Chip8 *c){
    // 0x3XNN: Skip next instruction if VX == NN
    if(c->V[X(c->opcode)] == IMM8(c->opcode)){
        c->pc += 2;
    }
    c->pc += 2;
}

void Chip8_Decode_4xxx(Chip8 *c){
    // 0x4XNN: Skip next instruction if VX != NN
    if(c->V[X(c->opcode)] != IMM8(c->opcode)){
        c->pc += 2;
    }
    c->pc += 2;
}

void Chip8_Decode_5xxx(Chip8 *c){
    // 0x5XY0: Skips next instruction if VX == VY
    if(c->V[X(c->opcode)] == c->V[Y(c->opcode)]){
        c->pc += 2;
    }
    c->pc += 2;
}

void Chip8_Decode_6xxx(Chip8 *c){
    // 0x6XNN: Set VX = NN
    c->V[X(c->opcode)] = IMM8(c->opcode);
    c->pc += 2;
}

void Chip8_Decode_7xxx(Chip8 *c){
    // 0x7XNN: VX = VX + NN (ignore carry)
    c->V[X(c->opcode)] += IMM8(c->opcode);
    c->pc += 2;
}

void Chip8_Decode_8xxx(Chip8 *c){
    unsigned short opcode = c->opcode;
    unsigned char *V = c->V;
    unsigned short sum_buffer;

    switch(IMM4(opcode)){
        case 0x0: // 0x8XY0: VX = VY
            V[X(opcode)] = V[Y(opcode)];
            break;
        case 0x1: // 0x8XY1: VX |= VY
            V[X(opcode)] |= V[Y(opcode)];
            break;
        case 0x2: // 0x8XY2: VX &= VY
            V[X(opcode)] &= V[Y(opcode)];
            break;
        case 0x3: // 0x8XY3: VX ^= VY
            V[X(opcode)] ^= V[Y(opcode)];
            break;
        case 0x4: // 0x8XY4: VX += VY (VF set if carry)
            sum_buffer = (unsigned short) V[X(opcode)] + (unsigned short) V[Y(opcode)];
            V[X(opcode)] = sum_buffer & 0x00FF;
            V[0xF] = (sum_buffer > 0xFF) ? 1 : 0;
            break;
        case 0x5: // 0x8XY5: VX -= VY (VF = NOT borrow)
            V[0xF] = (V[X(opcode)] > V[Y(opcode)]) ? 1 : 0;
            V[X(opcode)] -= V[Y(opcode)];
            break;
        case 0x6: // 0x8XY6: VX >>= 1 (VF = LSB(VX))
            V[0xF] = V[X(opcode)] & 0x01;
            V[X(opcode)] >>= 1;
            break;
        case 0x7: // 0x8XY7: VX = VY - VX (VF = NOT borrow)
            V[0xF] = (V[Y(opcode)] > V[X(opcode)]) ? 1 : 0;
            V[X(opcode)] = V[Y(opcode)] - V[X(opcode)];
            break;
        case 0xE: // 0x8XYE: VX <<= 1 (VF = MSB(VX))
            V[0xF] = (V[X(opcode)] & 0x80) >> 7;
            V[X(opcode)] <<= 1;
            break;
   } 
   c->pc += 2;
}

void Chip8_Decode_9xxx(Chip8 *c){
    // 0x9XY0: Skip next instruction if VX != VY
    if(c->V[X(c->opcode)] != c->V[Y(c->opcode)]){
        c->pc += 2;
    }
    c->pc += 2;
}

void Chip8_Decode_Axxx(Chip8 *c){
    // 0xANNN: I = NNN
    c->I = IMM12(c->opcode);
    c->pc += 2;
}

void Chip8_Decode_Bxxx(Chip8 *c){
    // 0xBNNN: Jump (pc = V0 + NNN)
    c->pc = c->V[0x0] + IMM12(c->opcode);
}

void Chip8_Decode_Cxxx(Chip8 *c){
    // 0xCXNN: VX = <random byte> & NN
    c->V[X(c->opcode)] = IMM8(c->opcode) & ((char) (rand() % 256));
    c->pc += 2;
}

void Chip8_Decode_Dxxx(Chip8 *c){
    // 0xDXYN: Draw 8xN sprite at location I at (VX, VY) on screen (set VF if collision)
    unsigned char pixel;
    int x, y;
    int xline, yline;

    x = c->V[X(c->opcode)];
    y = c->V[Y(c->opcode)];

    for(yline = 0; yline < IMM4(c->opcode); yline++){
        pixel = c->memory[c->I + yline];
        for(xline = 0; xline < 8; xline++){
            if((pixel & (0x80 >> xline)) != 0){
                if(c->gfx[(x + xline) + ((y + yline) * 64)] == 1){
                    c->V[0xF] = 1;
                }
                c->gfx[(x + xline) + ((y + yline) * 64)] ^= 1;
            }
        }
    }
    c->draw_flag = 1;
    c->pc += 2;
}

void Chip8_Decode_Exxx(Chip8 *c){
    switch(c->opcode & 0x00FF){
        case 0x9E: // 0xEX9E: Skip next inst if key stored in VX is pressed
            if(c->key_presses[c->V[X(c->opcode)]] == 1){
                c->pc += 2;
            }
            break;
        case 0xA1: // 0xEXA1: Skip next inst if key stored in VX is not pressed
            if(c->key_presses[c->V[X(c->opcode)]] == 0){
                c->pc += 2;
            }
            break;
    }
    c->pc += 2;
}

void Chip8_Decode_Fxxx(Chip8 *c){
    int i;
    unsigned char key; // Used in blocking keypress (0xFX0A)
    unsigned char temp; // Used in BCD (0xFX33)

    switch(c->opcode & 0x00FF){
        case 0x07: // 0xFX07: VX = delay_timer
            c->V[X(c->opcode)] = c->delay_timer;
            break;
        case 0x0A: // 0xFX0A: Blocks until keypress, then stores the press in VX
            key = blockingInput();
            if(key >= 0 && key < 16){
                c->V[X(c->opcode)] = key;
            }
            break;
        case 0x15: // 0xFX15: delay_timer = VX
            c->delay_timer = c->V[X(c->opcode)];
            break;
        case 0x18: // 0xFX18: sound_timer = VX
            c->sound_timer = c->V[X(c->opcode)];
            break;
        case 0x1E: // 0xFX1E: I += VX
            c->I += c->V[X(c->opcode)];
            break;
        case 0x29: // 0xFX29: I = <location of sprite for character in VX>
            c->I = (X(c->opcode)) * 5;
            break;
        case 0x33: // 0xFX33: Stores BCD of VX in I, I+1, I+2 (big-end)
            temp = c->V[X(c->opcode)];
            c->memory[c->I + 2] = temp % 10;
            c->memory[c->I + 1] = (temp / 10) % 10;
            c->memory[c->I]     = (temp / 100) % 10;
            break;
        case 0x55: // 0xFX55: Store V0-VX in memory starting at addr I
            for(i = 0; i <= X(c->opcode); i++){
                c->memory[c->I + i] = c->V[i];
            }
            break;
        case 0x65: // 0xFX65: Fill V0-VX from memory starting at addr I
            for(i = 0; i <= X(c->opcode); i++){
                c->V[i] = c->memory[c->I + i];
            }
            break;
    }
    c->pc += 2;
}