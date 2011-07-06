CC=gcc
CCFLAGS=-c -g -Wall
ION_LDFLAGS=-lsbp_ion -lbp -lbpP -ldtn2fw -lici -lncurses
DTN_LDFLAGS=-lsbp_dtn -ldtnapi
TARGET=client
OBJS=client.o bp.o list.o gui.o

all: ion

ion: $(OBJS)
	$(CC) $(ION_LDFLAGS) $(OBJS) -o $(TARGET)

dtn: $(OBJS)
	$(CC) $(DTN_LDFLAGS) $(OBJS) -o $(TARGET)

client.o: client.c
	$(CC) $(CCFLAGS) client.c
bp.o: bp.c
	$(CC) $(CCFLAGS) bp.c
list.o: list.c
	$(CC) $(CCFLAGS) list.c
gui.o: gui.c
	$(CC) $(CCFLAGS) gui.c

clean:
	rm -f *.o $(TARGET)

