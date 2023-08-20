main:
	gcc src/main/main.c src/main/prompt.c

debug:
	gcc -g -Wall -fsanitize=address,undefined src/main/main.c src/main/prompt.c

clean:
	rm *.out