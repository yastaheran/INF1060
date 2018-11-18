CC = gcc
CFLAGS = -g -Wall

all = oblig1

oblig1: oblig1_tests.o 2bg.o 2cd.o 2e.o
	$(CC) $(CFLAGS) -o oblig1 oblig1_tests.o 2bg.o 2cd.o 2e.o

oblig1_tests.o: oblig1_tests.c oppgave2.h
	$(CC) $(CFLAGS) -c oblig1_tests.c
	
2bg.o: 2bg.c
	$(CC) $(CFLAGS) -c 2bg.c
	
2cd.o: 2cd.c
	$(CC) $(CFLAGS) -c 2cd.c
	
2e.o: 2e.c
	$(CC) $(CFLAGS) -c 2e.c

clean: 
	rm oblig1 *.o *~