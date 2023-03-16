all: static shared_lib
static: # statyczna
	gcc -Wall -Wextra -c library.c
	ar rcs liblibrary.a library.o
	rm -f library.o

shared_lib: #współdzielona
	gcc -Wall -Wextra -fPIC -c library.c
	gcc -Wall -Wextra -shared library.o -o liblibrary.so
	rm -f library.o

clean: #czyszczenie
	rm -f *.o
	rm -f *.so
	rm -f *.a
