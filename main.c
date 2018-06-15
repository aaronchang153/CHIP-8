#include "Chip8.h"
#include <stdio.h>
#include <stdlib.h>


int main(){
    Chip8 *chip8 = Chip8_Create();

    Chip8_Destroy(chip8);    

    return 0;
}