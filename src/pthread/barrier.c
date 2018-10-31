//
// Created by gzq on 18-7-15.
//

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "barrier.h"
#include "random_uitls.h"


Barrier barrier;

void *work(void *rank) {
    int rnd = randint(0, 5);
    printf("thread %ld begin working ... about need %d seconds\n", (long) rank, rnd);
    sleep(rnd);
    printf("thread %ld wait barrier...\n", (long) rank);
    barrier_countdown(&barrier);
    printf("thread %ld continue working ...\n", (long) rank);
}

int main() {
    int thread_count = 20;

    barrier_init(&barrier, thread_count);

    pthread_t *pthread_handles = malloc(sizeof(pthread_t) * thread_count);

    for (long i = 0; i < thread_count; i++) {
        pthread_create(&pthread_handles[i], NULL, work, (void *) i);
    }

    for (long i = 0; i < thread_count; i++) {
        pthread_join(pthread_handles[i], NULL);
    }
    free(pthread_handles);
    barrier_destroy(&barrier);
}

int barrier_init(Barrier *barrier_p, int count) {
    if (count <= 0) {
        perror("count must greater than 0 !!!");
        return 1;

    }
    barrier_p->init_count = count;
    barrier_p->count = count;
    pthread_cond_init(&barrier_p->barrier, NULL);
    pthread_mutex_init(&barrier_p->mutex, NULL);
    return 0;
}

int barrier_destroy(Barrier *barrier_p) {
    pthread_cond_destroy(&barrier_p->barrier);
    pthread_mutex_destroy(&barrier_p->mutex);
    return 0;
}

int barrier_countdown(Barrier *barrier) {
    pthread_mutex_lock(&barrier->mutex);
    if (barrier->count > 1) {
        barrier->count--;
        while (pthread_cond_wait(&barrier->barrier, &barrier->mutex) != 0);
    } else if (barrier->count == 1) {
        pthread_cond_broadcast(&barrier->barrier);
        barrier->count == barrier->init_count;
    } else {
        perror("some thing must be wrong !!!");
        exit(1);
    }
    pthread_mutex_unlock(&barrier->mutex);
    return 0;
}