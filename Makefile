all:
	gcc -o nc nc.c -lncurses

clean:
	rm nc
