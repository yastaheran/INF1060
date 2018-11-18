CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -std=c11 -g


person: 05.c
	$(CC) $(CFLAGS) 05.c -o person

%:%.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(BINS)