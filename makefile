main:
	gcc -O2 src/main/*.c 

debug:
	gcc -g -Wall -fsanitize=address,undefined src/main/*.c

clean:
	rm *.out