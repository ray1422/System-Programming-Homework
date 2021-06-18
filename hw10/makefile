SHELL = /bin/bash
CC ?= gcc
CFLAGS = -g -pthread
SRC = $(wildcard *.c)
TARGET = $(patsubst %.c, %, $(SRC))

all: ${TARGET}

%:	%.c
	${CC} ${CFLAGS} $@.c -o $@ -lcrypt

clean:
	rm -f ${TARGET}
