SHELL=/bin/bash
CC?=gcc

all: ./nice_testing

./nice_testing: test.c
	@if ! [ "$(shell id -u)" = 0 ];then \
		echo "You are not root, run this target as root please";\
		exit 1;\
    fi
	${CC} $< -o $@
	setcap CAP_SYS_NICE+ep $@

clean:
	rm ./nice_testing
