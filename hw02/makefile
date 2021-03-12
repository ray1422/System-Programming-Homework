SHELL = /bin/bash
CC ?= gcc
CFLAGS = -g -pthread -O3
SRC = $(wildcard *.c)
TARGET = $(patsubst %.c, %, $(SRC))

all: ${TARGET}

%:	%.c
	${CC} ${CFLAGS} $@.c -o $@

myHole: ./hole
	./hole > /dev/null && echo "myHole generated." || (echo "Hole cannot be created" && false)

run: ./myHole ./mmap_cp2 ./mycp2
	time ./mmap_cp2 ./myHole ./myHole_mmap
	time ./mycp2 ./myHole ./myHole_mycp2
	ls -lash myHole*
	(cmp myHole myHole_mmap  && echo "mmap_cp2: src & dst are identical.") || (echo "copy failed" && false)
	(cmp myHole myHole_mycp2 && echo "mycp2   : src & dst are identical.") || (echo "copy failed" && false)

clean:
	rm -f ${TARGET}
	rm -f myHole*
