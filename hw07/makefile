SHELL = /bin/bash
CC ?= gcc
CFLAGS = -g -pthread -O3 -fsanitize=undefined
SRC = $(wildcard *.c)
TARGET = $(patsubst %.c, %, $(SRC))

all: ${TARGET}

%:	%.c
	${CC} ${CFLAGS} $@.c -o $@

clean:
	rm flock.db ${TARGET}
