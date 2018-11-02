//
// Created by gzq on 18-7-12.
//

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "time_utils.h"

int thread_count = 1000000;

pthread_t *pthread_handles;

void *func(void *rank) {}

int main(int argc, char **argv) {
    if (argc > 1) {
        thread_count = atoi(argv[1]);
    } else {
        thread_count = 100;
    };
    pthread_t *pthreadz_handles;
    pthread_handles = malloc(sizeof(pthread_t) * thread_count);
    timer_t time;
    start(&time);
    for (long i = 0; i < thread_count; i++) {
        pthread_create(&pthread_handles[i], NULL, func, (void *) i);
    }
    for (int i = 0; i < thread_count; i++) {
        pthread_join(pthread_handles[i], NULL);
    }
    end(&time);
    long interval = milli_interval(&time);
    printf("thread count = %d , cost time = %ld ms average time = %lf \n",
           thread_count, interval,
           (double) interval / thread_count);
    free(pthread_handles);
}
