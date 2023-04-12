RAPORT=report.txt

all: clean main

main:
	gcc -Wall -Werror -o zad2 zad2.c

test:
	make main
	./zad2 0.00002 5 > $(RAPORT)
	./zad2 0.00002 500 >> $(RAPORT)
	./zad2 0.00002 5000>> $(RAPORT)
	./zad2 0.0001 50000>> $(RAPORT)

clean:
	rm -f zad2 $(RAPORT)