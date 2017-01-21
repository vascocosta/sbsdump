CC=gcc
CFLAGS=-g -Wall -I.
LDFLAGS=-lcurl -lsqlite3
DEPS = log.h lookup.h macros.h message.h
OBJ = log.o lookup.o message.o sbsdump.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)

sbsdump: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o sbsdump
