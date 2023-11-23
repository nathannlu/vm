parser: main.c
	gcc -g -o a.out main.c

run: vm
	./a.out

