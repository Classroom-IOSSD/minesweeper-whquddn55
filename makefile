all: main

main: conio.o minesweeper.o
	gcc -g conio.o minesweeper.o -o main

conio.o: conio.c conio.h
	gcc -c conio.c -o conio.o

minesweeper.o: minesweeper.c
	gcc -c minesweeper.c -o minesweeper.o

clean:
	rm conio.o minesweeper.o main

.PHONY: all main clean
