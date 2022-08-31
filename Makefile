CC=gcc
CFLAGS=-Wall

all: main
	rm -f *.o main vgcore*

solver.o: src/solver.c
	$(CC) $(CFLAGS) -c $^

main: main.c solver.o
	$(CC) $(CFLAGS) $^ -o $@

