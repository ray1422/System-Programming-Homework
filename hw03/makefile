SHELL = /bin/bash
CC ?= gcc
CFLAGS = -g -pthread -O3 -fsanitize=undefined
SRC = $(wildcard *.c)
TARGET = $(patsubst %.c, %, $(SRC))

all: ${TARGET}

%:	%.c
	${CC} ${CFLAGS} $@.c -o $@

mount:
	sudo mount -oremount,mand /

chmod:
	chmod g+s lockf.db
	chmod g-x lockf.db

run_flock: ./flock
	for i in {0..3};			\
	do							\
	    ./flock &				\
	    set pids[{$$i}]={$$!};	\
	done &&						\
	for pid in {0..3}; do wait $${pids[$$pid]}; done

run_lockf: chmod ./lockf 
	./lockf
	./lockf
	./lockf
	./lockf

clean:
	rm flock.db ${TARGET}
