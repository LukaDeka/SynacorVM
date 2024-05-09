main: main.c main.h output.c output.h Makefile
	clang -gdwarf-4 -fsanitize=address -Wall -Wextra -Werror -pedantic main.c output.c -o bin/main