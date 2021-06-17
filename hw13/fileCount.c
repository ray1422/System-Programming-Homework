//usage: pipe4-3 killgrp或 pipe4-3 killproc
//結果相當於執行： ls | sort
#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int pid1, pid2, killgrp = 1;

void child_int(int signum) {
    puts("child");
    _exit(0);
}

void parent_int(int signum) {
    puts("parent");
    _exit(0);
}

int main(int argc, char *argv[]) {
    int pipefd[2];
    int ret, wstat;
    int grp_id = getpgid(getpid());
    if (argc != 2) {
        printf("usage %s <DIR>\n", argv[0]);
        exit(0);
    }
    killgrp = 1;

    pipe(pipefd);
    pid1 = fork();  //產生第一個child
    if (pid1 == 0) {
        printf("1st child's group id is %d\n", getpgrp());
        //setpgid(0, grp_id);
        printf("1st child's new group id is %d\n", getpgrp());
        close(1);                                 //關閉stdout
        dup(pipefd[1]);                           //將pipefd[1]複製到stdout
        close(pipefd[1]);                         //將沒用到的關閉
        close(pipefd[0]);                         //將沒用到的關閉
        
        execlp("ls", "ls", "-R", argv[1], NULL);  //執行ls，ls會將東西藉由stdout輸出到pipefd[1]
        exit(0);
    } else {
        // printf("1st child's pid = %d\n", pid1);
    }

    if (pid1 > 0) {
        pid2 = fork();  //產生第二個child
        if (pid2 == 0) {
            printf("2nd child's group id is %d\n", getpgrp());
            //setpgid(0, grp_id);  //第二個child加入第一個child的group
            printf("2nd child's new group id is %d\n", getpgrp());
            close(0);                        //關閉stdin
            dup(pipefd[0]);                  //將pipefd[0]複製到stdin
            close(pipefd[1]);                //將沒用到的關閉
            close(pipefd[0]);                //將沒用到的關閉
            execlp("wc", "wc", "-l", NULL);  //執行wc，wc將透過stdin從pipefd[0]讀入資料
        } else {
            // printf("2nd child's pid = %d\n", pid2);
        }
    }
    //parent一定要記得關掉pipe不然wc不會結束（因為沒有接到EOF）
    close(pipefd[0]);
    close(pipefd[1]);
    /*parent註冊signal handler*/

    printf("child %d\n", wait(&wstat));
    printf("child %d\n", wait(&wstat));
}