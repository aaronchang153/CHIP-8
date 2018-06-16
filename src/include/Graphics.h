#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "SDL.h"
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH  64
#define SCREEN_HEIGHT 32

#define PIXEL_SIZE     8

#define WINDOW_NAME "CHIP-8"


typedef struct{
    SDL_Window   *window;
    SDL_Renderer *renderer;
} Graphics;

Graphics *Graphics_Create();

char Graphics_Init(Graphics *g);

void Graphics_Destroy(Graphics *g);

void Graphics_Draw_Screen(Graphics *g, unsigned char *screen);

#endif // GRAPHICS_H