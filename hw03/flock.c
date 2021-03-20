#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

/*
ret = flock(fd, LOCK_EX);
ret = flock(fd, LOCK_UN);
*/

int main(void) {
    int fd = open("flock.db", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    assert(fd != -1);
    flock(fd, LOCK_EX);
    unsigned short n = 3499;
    off_t off = lseek(fd, 0, SEEK_END);
    char buf[4096];
    if (off) {
        lseek(fd, -2, SEEK_CUR);
        printf("off: %ld\n", off);
        read(fd, buf, 2);
        n = *((unsigned short *)buf);
    }
    n++;
    printf("starts from %d\n", n);
    for (int i = 0; i < 3000; i++) {
        write(fd, &n, 2);
        lseek(fd, n, SEEK_CUR);
        n++;
        usleep(1000);
    }
    flock(fd, LOCK_UN);
    close(fd);
}
