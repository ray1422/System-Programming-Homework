#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    for (int i = 0; i < 10; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            execl("ls", "ls", "-alh", "/");
            exit(0);
        } else {
            continue;
        }
    }
    while(1);
}