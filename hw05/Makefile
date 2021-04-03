SHELL = /bin/bash
CC = gcc
CFLAGS = -g -pthread -z lazy -fsanitize=undefined -fsanitize=address
SRC = $(wildcard *.c)
TARGET = $(patsubst %.c, %, $(SRC))

all: ${TARGET}

%:	%.c
	if [ "$$EUID" -ne 0 ];	then echo "Please run as root";	exit 1;	fi
	${CC} ${CFLAGS} $@.c -o $@
	chmod +s $@

clean:
	rm ${TARGET}
