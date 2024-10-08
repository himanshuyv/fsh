main:
	gcc -O3 src/main/*.c src/main/util/*.c src/main/commands/*.c

debug:
	gcc -O0 -g -Wall -Werror -fsanitize=address,undefined -DDEBUG src/main/*.c src/main/util/*.c src/main/commands/*.c

clean:
	rm *.out .osn_shell_history