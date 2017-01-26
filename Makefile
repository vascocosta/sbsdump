CC=gcc
CFLAGS=-g -Wall -I.
LDFLAGS=-lsqlite3
DEPS = database.h macros.h message.h
OBJ = database.o message.o sbsdump.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)

sbsdump: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o sbsdump
