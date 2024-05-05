main: main.c main.h ./etc/moves.txt Makefile
	clang -g main.c -o bin/main -Wall -lm
