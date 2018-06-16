#ifndef INPUT_H
#define INPUT_H

#include "SDL.h"

unsigned char getKeypadValue(int index);

unsigned char mapKeyPress(int scancode);

unsigned char blockingInput();

#endif // INPUT_H