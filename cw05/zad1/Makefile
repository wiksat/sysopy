RAPORT=report.txt

all: clean main

main:
	gcc -Wall -Werror -o zad1 zad1.c

test:
	./zad1 root example_title example_content
	./zad1 sender
	./zad1 date
clean:
	rm -f zad1