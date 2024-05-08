main: main.c main.h output.c output.h Makefile
	clang -g main.c output.c -o bin/main -Wall -lm