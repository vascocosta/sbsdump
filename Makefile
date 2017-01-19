CC=gcc
CFLAGS=-g -Wall -I.
LDFLAGS=-lcurl
DEPS = lookup.h macros.h message.h
OBJ = lookup.o message.o sbsdump.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)

sbsdump: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o sbsdump
