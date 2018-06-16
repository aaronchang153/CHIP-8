INCLUDE = -I src/include -I SDL2/include/SDL2 -L SDL2/lib -L SDL2/bin
LFLAGS = -Wall -g -lmingw32 -lSDL2main -lSDL2 #-Wl,-subsystem,windows
CFLAGS = -Wall -c -g

all : Chip8

Chip8.o : src/Chip8.c src/include/Chip8.h
	gcc $(INCLUDE) $(CFLAGS) src/Chip8.c -o obj/Chip8.o

Graphics.o : src/Graphics.c src/include/Graphics.h
	gcc $(INCLUDE) $(CFLAGS) src/Graphics.c -o obj/Graphics.o

Input.o : src/Input.c src/include/Input.h
	gcc $(INCLUDE) $(CFLAGS) src/Input.c -o obj/Input.o

main.o : main.c
	gcc $(INCLUDE) $(CFLAGS) main.c -o obj/main.o

Chip8 : Chip8.o Graphics.o Input.o main.o
	gcc $(INCLUDE) obj/main.o obj/Chip8.o obj/Graphics.o obj/Input.o $(LFLAGS) -o bin/Chip8.exe