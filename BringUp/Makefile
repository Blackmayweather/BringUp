CC = gcc
Link = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

matrix.o: matrix.c
	$(CC) -c $^ -o bin/$@
main: main.c bin/matrix.o
	gcc $^ -o bin/$@ $(Link)