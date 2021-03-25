#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
const int LINE_LIM = 80;
int main(int argc, char **argv) {
	if (argc < 4) {
		fprintf(stderr, "usage: %s <src> <dst> <buf_size>\n", argv[0]);
		return 1;
	}
	FILE *src = fopen(argv[1], "r");
	assert(src != NULL);
	FILE *dst = fopen(argv[2], "w+");
	assert(dst != NULL);
	const size_t buf_size = (size_t)atoi(argv[3]);
	setvbuf(src, malloc(buf_size), _IOFBF, buf_size);
	setvbuf(dst, malloc(buf_size), _IOFBF, buf_size);
	static char buf[8192];
	int line_cur = 0;
	while (~fscanf(src, "%s[^\n]", buf)) {
		char next = fgetc(src);
		const int len = strlen(buf);
		if (line_cur + len > LINE_LIM) {
			// break line
			fwrite("\n", 1, 1, dst);
			line_cur = 0;
		}
		fwrite(buf, len, 1, dst);
		fwrite(" ",   1, 1, dst);
		line_cur++;
		line_cur += len;
		if (next == '\n') {
			fwrite("\n", 1, 1, dst);
			line_cur = 0;
		}
	}
}
