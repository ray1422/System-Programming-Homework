#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#define __USE_GNU
#include <unistd.h>
#include <sched.h>
#include <pthread.h>

sig_atomic_t alrm = 1;

void alrm_handler() {
    alrm = 0;
}
int main() {
    pid_t pid = fork();
    cpu_set_t mask;  //CPU核的集合
    cpu_set_t get;   //获取在集合中的CPU
    CPU_ZERO(&mask);    //置空
    CPU_SET(0, &mask);   // 将当前线程和CPU绑定
    if(sched_setaffinity(0, sizeof(mask), &mask)) {
        perror("CPU 親和力設定失敗");
        abort();
    }

    if (pid == 0) { 
        nice(-5);
    } else {
    }

    int l = 0;
    signal(SIGALRM, alrm_handler);
    alarm(1);
    while(alrm) {
        l++;
    }
    if (pid == 0) {
        printf("c*: %d\n", l);
    } else {
        printf("p*: %d\n", l);
    }
}

