RAPORT=report.txt

all: clean main

main:
	gcc -Wall -Werror -o zad3 zad3.c
	gcc -Wall -Werror -o integral integral.c

test:
	make main
	./zad3 0.00002 5 > $(RAPORT)
	./zad3 0.00002 500 >> $(RAPORT)
	./zad3 0.00002 5000>> $(RAPORT)
	./zad3 0.0001 50000>> $(RAPORT)

clean:
	rm -f zad3 integral $(RAPORT)