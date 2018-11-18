CC := gcc
CFLAGS := -O0 -Wall -pg -g3 -DDEBUG

.PHONY: clean

ifish: builtin.o history.o safefork.o pidlist.o ifish.o

clean:
	rm -fv *.o ifish

# Dependencies per .c-file

builtin.o:  builtin.c  builtin.h  common.h history.h ifish.h
history.o:  history.c  history.h  common.h 
safefork.o: safefork.c safefork.h common.h 
pidlist.o:  pidlist.c  pidlist.h  common.h
ifish.o:    ifish.c    ifish.h    common.h builtin.h safefork.h history.h pidlist.h 
