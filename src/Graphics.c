#include "Graphics.h"

Graphics *Graphics_Create(){
    Graphics *graphics = NULL;
    graphics = (Graphics *) malloc(sizeof(Graphics));
    if(!Graphics_Init(graphics)){
        // If initialization fails
        Graphics_Destroy(graphics);
        graphics = NULL;
    }
    return graphics;
}

char Graphics_Init(Graphics *g){
    char success = 1;
    if(g == NULL){
        success = 0;
    }
    else{
        if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
            printf("Error initializing SDL.\n");
            success = 0;
        }
        else{
            g->window = SDL_CreateWindow(WINDOW_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                SCREEN_WIDTH * PIXEL_SIZE, SCREEN_HEIGHT * PIXEL_SIZE, SDL_WINDOW_SHOWN);
            if(g->window == NULL){
                printf("Error creating window.\n");
                success = 0;
            }
            else{
                g->renderer = SDL_CreateRenderer(g->window, -1, SDL_RENDERER_ACCELERATED);
                if(g->renderer == NULL){
                    printf("Error creating renderer.\n");
                    success = 0;
                }
            }
        }
    }
    return success;
}

void Graphics_Destroy(Graphics *g){
    if(g != NULL){
        if(g->renderer != NULL){
            SDL_DestroyRenderer(g->renderer);
        }
        if(g->window != NULL){
            SDL_DestroyWindow(g->window);
        }
        free(g);
    }
    SDL_Quit();
}

void Graphics_Draw_Screen(Graphics *g, unsigned char *screen){
    int i, j;
    SDL_Rect pixel;

    SDL_SetRenderDrawColor(g->renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(g->renderer);

    SDL_SetRenderDrawColor(g->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    for(j = 0; j < SCREEN_HEIGHT; j++){
        for(i = 0; i < SCREEN_WIDTH; i++){
            if(screen[i + (j * SCREEN_WIDTH)] != 0){
                pixel.x = i * PIXEL_SIZE;
                pixel.y = j * PIXEL_SIZE;
                pixel.w = PIXEL_SIZE;
                pixel.h = PIXEL_SIZE;

                SDL_RenderFillRect(g->renderer, &pixel);
            }
        }
    }
    SDL_RenderPresent(g->renderer);
}