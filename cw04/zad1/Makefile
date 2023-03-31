CC = gcc -Wall -Werror

main: clean
	$(CC) main.c -o main
	$(CC) exec_ignore.c -o exec_ignore
	$(CC) exec_mask.c -o exec_mask
	$(CC) exec_pending.c -o exec_pending
	
test : main test_child test_exec
	
test_child : main
	echo "ignore" >> raport2.txt 
	./main ignore >> raport2.txt 
	echo "\n" 	>> raport2.txt 
	echo "handler" >> raport2.txt 
	./main handler >> raport2.txt 
	echo "\n" 	>> raport2.txt 
	echo "mask" >> raport2.txt 
	./main mask >> raport2.txt 
	echo "\n" 	>> raport2.txt 
	echo "pending" >> raport2.txt 
	./main pending >> raport2.txt 
	echo "\n" 	>> raport2.txt 
test_exec: exec_ignore exec_mask exec_pending
	echo "execIgnore" >> raport2.txt 
	./main execIgnore >> raport2.txt 
	echo "\n" 	>> raport2.txt 
	echo "execMask" >> raport2.txt 
	./main execMask >> raport2.txt 
	echo "\n" 	>> raport2.txt 
	echo "execPending" >> raport2.txt 
	./main execPending >> raport2.txt 
	echo "\n" 	>> raport2.txt 
	
clean:
	rm -f main exec_ignore exec_mask exec_pending