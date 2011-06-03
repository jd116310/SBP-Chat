CC=gcc
CCFLAGS=-c -g -O2 -Wall -Werror 

SOURCES=${wildcard *.c}
HEADERS=${wildcard *.h}
OBJECTS=${SOURCES:.c=.o}

LDFLAGS=-lsbp_ion -lbp -lbpP -ldtn2fw -lici -lncurses #-lecos -lipcx 

all: client server

${OBJECTS}: ${HEADERS} Makefile

client: $(OBJECTS)

server:

run:
	./client

clean:
	rm client *.o
