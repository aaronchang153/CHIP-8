#include "Chip8.h"
#include "Graphics.h"
#include "Input.h"
#include "SDL.h"
#include <stdio.h>
#include <stdlib.h>


int SDL_main(int argc, char *argv[]){
    Chip8 *chip8 = Chip8_Create();
    if(chip8 != NULL){
        Graphics *display = Graphics_Create();
        if(display != NULL){

            Chip8_Load_Application(chip8, "games/TANK");

            SDL_Event event;
            int key;

            char running = 1;
            while(running){
                if(SDL_PollEvent(&event)){
                    switch(event.type){
                        case SDL_QUIT:
                            running = 0;
                            break;
                        case SDL_KEYDOWN:
                        case SDL_KEYUP:
                            key = mapKeyPress(event.key.keysym.scancode);
                            if(key >= 0){
                                chip8->key_presses[key] = (event.type == SDL_KEYDOWN) ? 1 : 0;
                            }
                            break;
                    };
                }
                Chip8_EmulateCycle(chip8);
                if(chip8->draw_flag == 1){
                    Graphics_Draw_Screen(display, chip8->gfx);
                    chip8->draw_flag = 0;
                }
                SDL_Delay(5);
            }

            Graphics_Destroy(display);
        }
        Chip8_Destroy(chip8);    
    }
    return 0;
}