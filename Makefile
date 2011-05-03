all: client server

client: 
	gcc -c list.c
	gcc -c client.c
	gcc -lncurses -o client client.o list.o

server:

run:
	./client

clean:
	rm client *.o
