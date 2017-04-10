all: main

main: conio.o minsweeper.o
	gcc -g coino.o minsweeper.o -o main

conio.o: coino.c conio.h
	gcc -c conio.c -o conio.o

minesweeper.o: minesweeper.c
	gcc -c minesweeper.c -o minesweeper.o

clean:
	rm conio.o minesweeper.o main

.PHONY: all main clean
