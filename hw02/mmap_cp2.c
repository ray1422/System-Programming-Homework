#define _GNU_SOURCE
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#define BUF_SIZE 8192  // 8192
int main(int argc, char* argv[]) {
    int inputFd, outputFd;

    ssize_t numIn, numOut;
    off_t data_off = 0, hole_off = 0, cur_off = 0;
    off_t fileSize, blockSize, pos = 0;
    static char buffer[BUF_SIZE];

    inputFd = open(argv[1], O_RDONLY);
    if (inputFd == -1) {
        perror("cannot open the file for read");
        exit(1);
    }

    outputFd = open(argv[2], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (outputFd == -1) {
        perror("canot open the file for write");
        exit(1);
    }

    fileSize = lseek(inputFd, 0, SEEK_END);
    lseek(inputFd, 0, SEEK_SET);
    printf("file size = %ld\n", fileSize);

    char* inputPtr = mmap(NULL, fileSize, PROT_READ, MAP_SHARED, inputFd, 0);
    perror("mmap");
    printf("inputPtr = %p\n", inputPtr);
    //assert(madvise(inputPtr, fileSize, MADV_SEQUENTIAL|MADV_WILLNEED|MADV_HUGEPAGE)==0);

    ftruncate(outputFd, fileSize);
    char* outputPtr = mmap(NULL, fileSize, PROT_WRITE, MAP_SHARED, outputFd, 0);
    perror("mmap, output");
    printf("outputPtr = %p\n", outputPtr);
    //madvise(inputPtr, fileSize, MADV_SEQUENTIAL|MADV_WILLNEED|MADV_HUGEPAGE);

    printf("memory copy\n");
    time_t timer1, timer2;
    timer1 = time(NULL);
    // start copying
    // memcpy(outputPtr, inputPtr, fileSize);
    while (1) {
        cur_off = lseek(inputFd, cur_off, SEEK_DATA);
        data_off = cur_off;
        cur_off = lseek(inputFd, cur_off, SEEK_HOLE);
        hole_off = cur_off;
        //第一種情況，資料在前面，洞在後面，不用特別處理
        //第二種情況，洞在前面，資料在後面，處理一下
        if (data_off > hole_off) {
            //現在是這樣：
            //  ...............data data data data data....
            //  ^hole_off      ^data_off=cur_off
            //因為cur_off已經移到後面了，所以下一輪執行會變成
            //  ...............data data data data data....
            //                 ^data_off               ^hole_off=curoff
            continue;
        }
        //至此，data_off一定在前面，hole_off一定在後面
        blockSize = hole_off - data_off;
        memcpy(outputPtr + data_off, inputPtr + data_off, blockSize);
        lseek(inputFd, data_off + blockSize, SEEK_SET);
        lseek(outputFd, data_off + blockSize, SEEK_SET);
        if (lseek(outputFd, 0, SEEK_CUR) == fileSize) break;
    }

    timer2 = time(NULL);

    printf("time(memcpy) = %ld sec \n", timer2 - timer1);

    assert(munmap(inputPtr, fileSize) == 0);
    assert(munmap(outputPtr, fileSize) == 0);

    assert(close(inputFd) == 0);
    assert(close(outputFd) == 0);

    return (EXIT_SUCCESS);
}
