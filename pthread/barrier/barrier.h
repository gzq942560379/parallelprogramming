//
// Created by gzq on 18-7-15.
//

#ifndef PTHREAD_DEMO_BARRIER_H
#define PTHREAD_DEMO_BARRIER_H


#include <sys/param.h>

typedef struct Barrier {
    int init_count;
    int count;
    pthread_cond_t barrier;
    pthread_mutex_t mutex;
} Barrier;

int barrier_init(Barrier *barrier_p, int count);

int barrier_destroy(Barrier *barrier_p);

int barrier_countdown(Barrier *barrier);

#endif //PTHREAD_DEMO_BARRIER_H
