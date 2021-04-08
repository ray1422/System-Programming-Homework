SHELL = /bin/bash
CC ?= gcc
CFLAGS ?= -g -z lazy
LD_FLAGS ?= -pthread
TARGETS =cp ls cat chown
OUTPUT_DIR=bin
MAIN=${OUTPUT_DIR}/lazybox
OUTPUTS = $(patsubst %,${OUTPUT_DIR}/%,${TARGETS})

all: ${MAIN} ${OUTPUTS}

${OUTPUTS}: %: ${MAIN} main.c
	ln -sf ${PWD}/${MAIN} ${PWD}/$@

${MAIN}: main.c ${OUTPUT_DIR}
	${CC} ${CFLAGS} ${LD_FLAGS} main.c -o ${MAIN}

${OUTPUT_DIR}:
	mkdir ${OUTPUT_DIR}