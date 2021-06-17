#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sig_atomic_t stop = 0;

int int_handler(int sig) {
    stop = 1;
}

const int SYNC_FREQ = 100000;

struct Param {
    pthread_mutex_t *lock;
    unsigned long long n;
    unsigned long long *total;
    unsigned long long *inCir;
};

void *job(void *param_) {
    struct Param *param = param_;
    unsigned long long lastN = 0;
    unsigned long long cir = 0;
    struct drand48_data dr_buf;
    srand48_r((long int)&cir, &dr_buf);
    for (unsigned long long i = 0; i < param->n; i++) {
        double x, y;
        drand48_r(&dr_buf, &x);
        drand48_r(&dr_buf, &y);
        if (x * x + y * y < 1.) ++cir;
        if (i % SYNC_FREQ == 0 || i ==  param->n - 1) {
            pthread_mutex_lock(param->lock);
            *(param->total) += i - lastN + 1;
            *(param->inCir) += cir;
            // printf("%llu\n", *(param->total));
            pthread_mutex_unlock(param->lock);
            lastN = i + 1;
            cir = 0;
        }
    }
}

int main(int argc, char *argv[]) {
    int nt = 0;
    if (argc == 2) {
        nt = atoi(argv[1]);
    }
    if (nt <= 0 || nt > 1000) {
        nt = 4;
    }
    printf("threads: %d\n", nt);
    pthread_t threads[1024];
    unsigned long long inCircle = 0, total = 0, target = (unsigned long long)1 << 35;
    target = target * nt / nt;
    pthread_mutex_t lock;
    pthread_mutex_init(&lock, NULL);
    struct Param attr = {
        .inCir = &inCircle,
        .total = &total,
        .n = target / nt,
        .lock = &lock
    };
    for (int i = 0; i < nt; i++) {
        pthread_create(threads + i, NULL, job, (void *)&attr);
    }
    
    while (!stop && total < target) {
        sleep(3);
        const long double PI = (long double)inCircle * 4 / (long double)total;
        printf("PI: %2.9Lf\n", PI);
        printf("%018llu\n%018llu\n", total, target);
    }
    for (int i = 0; i < nt; i++) {
        pthread_join(threads[i], NULL);
    }
}