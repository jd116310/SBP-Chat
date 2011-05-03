all: client server

client: client.c gui.c list.c
	gcc -c client.c
	gcc -c gui.c
	gcc -c list.c
	gcc -lncurses -o client client.o list.o gui.o

server:

run:
	./client

clean:
	rm client *.o
