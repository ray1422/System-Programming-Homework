SHELL = /bin/bash
CC = gcc
CFLAGS = -g -pthread -z lazy
SRC = $(wildcard *.c)
TARGET = $(patsubst %.c, %, $(SRC))

all: ${TARGET}

%:	%.c
	${CC} ${CFLAGS} $@.c -o $@

clean:
	rm ${TARGET}
