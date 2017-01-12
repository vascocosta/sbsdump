CC=gcc
CFLAGS=-g -Wall -I.
DEPS = macros.h message.h
OBJ = message.o sbsdump.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

sbsdump: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o sbsdump
