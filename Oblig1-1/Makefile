CC = gcc
CFLAGS = -Wall -g

.PHONY: all clean

all: oppgave1 oppgave2

oppgave1: oppgave1.c oppg1.h
	$(CC) $(CFLAGS) -o 1 oppgave1.c

oppgave2: oppgave2.c oppg2.h
	$(CC) $(CFLAGS) -o 2 oppgave2.c
	
clean:
	rm -f 1 2