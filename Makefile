CC=gcc
CFLAGS=-I.
DEPS = message.h

%.o: %.c $(DEPS)
	$(CC) -c -g -o $@ $< $(CFLAGS)

sbsdump: sbsdump.o message.o 
	gcc -g -o sbsdump sbsdump.o message.o -I.
