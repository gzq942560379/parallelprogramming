//
// Created by gzq on 18-7-13.
//

#ifndef PTHREAD_DEMO_RWLOCK_H
#define PTHREAD_DEMO_RWLOCK_H


#include <stdbool.h>
#include <sys/param.h>

typedef struct RWLock {
    int active_read_count;
    int wait_read_count;
    bool has_active_write;
    int wait_write_count;
    pthread_cond_t read;
    pthread_cond_t write;
    pthread_mutex_t mutex;
} RWLock;

int rwlock_init(RWLock *rwlock);

int rwlock_destroy(RWLock *rwlock);

int rwlock_rdlock(RWLock *rwlock);

int rwlock_wrlock(RWLock *rwlock);

int rwlock_unlock(RWLock *rwlock);

#endif //PTHREAD_DEMO_RWLOCK_H
