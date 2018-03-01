HEADERS = list.h

default: list

list.o: list.c $(HEADERS)
	gcc -c list.c -o list.o

main.o: main.c $(HEADERS)
	gcc -c main.c -o main.o

list: main.o list.o
	gcc main.o list.o -o main

clean:
	-rm -f list.o main.o
	-rm -f main
