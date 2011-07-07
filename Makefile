CC=g++
CCFLAGS=-c -g -Wall
ION_LDFLAGS=-lsbp_ion -lbp -lbpP -ldtn2fw -lici -lncurses
DTN_LDFLAGS=-lsbp_dtn -ldtnapi
TARGET=client
TARGET2=server
OBJS=client.o bp.o list.o gui.o
OBJS2=server.o

all: ion

ion: $(OBJS) $(OBJS2)
	$(CC) $(ION_LDFLAGS) $(OBJS) -o $(TARGET)
	$(CC) $(ION_LDFLAGS) $(OBJS2) -o $(TARGET2)

dtn: $(OBJS)
	$(CC) $(DTN_LDFLAGS) $(OBJS) -o $(TARGET)

client.o: client.cpp
	$(CC) $(CCFLAGS) client.cpp
bp.o: bp.cpp
	$(CC) $(CCFLAGS) bp.cpp
list.o: list.cpp
	$(CC) $(CCFLAGS) list.cpp
gui.o: gui.cpp
	$(CC) $(CCFLAGS) gui.cpp
server.o: server.cpp
	$(CC) $(CCFLAGS) server.cpp

clean:
	rm -f *.o $(TARGET) $(TARGET2)

