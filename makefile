main:
	gcc -O2 src/main/*.c src/main/util/*.c

debug:
	gcc -g -Wall -fsanitize=address,undefined src/main/*.c src/main/util/*.c

clean:
	rm *.out .osn_shell_history