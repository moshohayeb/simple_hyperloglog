P=hyperloglog
OBJECTS=hll.c main.c
CFLAGS=-Wall -O3
LDFLAGS=-lm
CC=gcc

$(P): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

clean:
	rm -f $(P)
