NAME=repl
all: static shared dynamic

static:
	gcc -Wall -Wextra -c repl.c
	gcc -Wall -Wextra repl.o ../zad1/liblibrary.a -o $(NAME)Static
	gcc -Wall -Wextra repl.o ../zad1/liblibrary.a -O -o $(NAME)StaticO
	gcc -Wall -Wextra repl.o ../zad1/liblibrary.a -O2 -o $(NAME)StaticO2
	gcc -Wall -Wextra repl.o ../zad1/liblibrary.a -Os -o $(NAME)StaticOs
	rm -f repl.o

shared:
	gcc -Wall -Wextra repl.c -L ../zad1 -llibrary -Wl,-rpath=../zad1/ -o $(NAME)Shared
	gcc -Wall -Wextra repl.c -L ../zad1 -llibrary -Wl,-rpath=../zad1/ -O -o $(NAME)SharedO
	gcc -Wall -Wextra repl.c -L ../zad1 -llibrary -Wl,-rpath=../zad1/ -O2 -o $(NAME)SharedO2
	gcc -Wall -Wextra repl.c -L ../zad1 -llibrary -Wl,-rpath=../zad1/ -Os -o $(NAME)SharedOs

dynamic:
	gcc -Wall -Wextra repl.c -o $(NAME)Dynamic -ldl -D DLL -I.
	gcc -Wall -Wextra repl.c -O -o $(NAME)DynamicO -ldl -D DLL -I.
	gcc -Wall -Wextra repl.c -O2 -o $(NAME)DynamicO2 -ldl -D DLL -I.
	gcc -Wall -Wextra repl.c -Os -o $(NAME)DynamicOs -ldl -D DLL -I.

clean:
	rm -f $(NAME)Static $(NAME)Shared $(NAME)Dynamic $(NAME)StaticO $(NAME)SharedO $(NAME)DynamicO $(NAME)StaticO2 $(NAME)SharedO2 $(NAME)DynamicO2 $(NAME)StaticOs $(NAME)SharedOs $(NAME)DynamicOs