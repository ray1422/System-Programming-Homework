#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();
    if (pid == 0) {
        for (int i = 0; i < 100; i++) {
            pid_t pid2 = fork();
            if (pid2 == 0) {
                printf("start child %d\n", i);
                sleep(i);
                printf("end child %d\n", i);
                exit(0);
            } else {
                continue;
            }
        }
        exit(0);

    } else {
        wait(NULL);
        while (1)
            ;
    }
}