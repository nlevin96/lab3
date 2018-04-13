
all: main

main: toy_printf.o main[1].o
	gcc -g -Wall -o main toy_printf.o main[1].o

toy_printf.o: toy_printf.c
	gcc -g -Wall -c -o toy_printf.o toy_printf.c 

main.o: main[1].c
	gcc -g -Wall -c -o main[1].o main[1].c 

.PHONY: clean

clean: 
	rm -f *.o main