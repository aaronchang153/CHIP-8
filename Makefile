INCLUDE = -I src/include #-I ./SDL2 -L ./lib
LFLAGS = -Wall -g -lmingw32# -lSDL2main -lSDL2 #-Wl,-subsystem,windows
CFLAGS = -Wall -c -g

all : Chip8

Chip8.o : src/Chip8.c src/include/Chip8.h
	gcc $(INCLUDE) $(CFLAGS) src/Chip8.c -o obj/Chip8.o

main.o : main.c
	gcc $(INCLUDE) $(CFLAGS) main.c -o obj/main.o

Chip8 : main.o Chip8.o
	gcc $(INCLUDE) obj/main.o obj/Chip8.o $(LFLAGS) -o bin/Chip8.exe