all:
	gcc -c list.c
	gcc -c nc.c
	gcc -lncurses -o nc nc.o list.o

clean:
	rm nc *.o
