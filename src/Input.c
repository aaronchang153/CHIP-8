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

unsigned char getKeypadValue(int index){
    if(index >= 0 && index < 16){
        return keypad[index];
    }
    return -1;
}

unsigned char mapKeyPress(int scancode){
    char key_index = -1;
    int i;
    for(i = 0; i < 16; i++){
        if(keymap[i] == scancode){
            key_index = i;
            break;
        }
    }
    return key_index;
}

unsigned char blockingInput(){
    SDL_Event e;
    int key_index;
    while(SDL_PollEvent(&e)){
        if(e.type == SDL_KEYDOWN){
            key_index = mapKeyPress(e.key.keysym.scancode);
            if(key_index >= 0 && key_index < 16){
                return keypad[key_index];
            }
        }
    }
    return 0;
}