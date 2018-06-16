#include "Input.h"

int keypad[] = 
    {
        0x1, 0x2, 0x3, 0xC,
        0x4, 0x5, 0x6, 0xD,
        0x7, 0x8, 0x9, 0xE,
        0xA, 0x0, 0xB, 0xF
    };

int keymap[] = 
    {
        SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_4,
        SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_R,
        SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_F,
        SDL_SCANCODE_Z, SDL_SCANCODE_X, SDL_SCANCODE_C, SDL_SCANCODE_V
    };


int mapKeyPress(int scancode){
    char key = -1;
    int i;
    for(i = 0; i < 16; i++){
        if(keymap[i] == scancode){
            key = keypad[i];
            break;
        }
    }
    return key;
}

int blockingInput(){
    SDL_Event e;
    char key;
    while(SDL_PollEvent(&e)){
        if(e.type == SDL_KEYDOWN){
            if((key = mapKeyPress(e.key.keysym.scancode)) >= 0){
                return key;
            }
        }
    }
    return -1;
}