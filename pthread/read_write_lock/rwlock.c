//
// Created by gzq on 18-7-13.
//

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "rwlock.h"
#include "../../util/random_utils/random_uitls.h"

long counter = 0;
RWLock rwLock;
const long loop_count = 100000;


void *func(void *rank) {
    long my_counter = 0;
    for (int i = 0; i < loop_count; i++) {
        double rnd = random_double();
        if (rnd < 0.7) {
            rwlock_rdlock(&rwLock);
            printf("read counter == %ld\n", counter);
            rwlock_unlock(&rwLock);
        } else {
            my_counter++;
            rwlock_wrlock(&rwLock);
            long read_counter = counter;
            counter++;
            printf("read counter = %ld write counter == %ld \n", read_counter, counter);
            rwlock_unlock(&rwLock);
        }
    }
    return (void *) my_counter;
}

int main() {
    srand(time(NULL));
    rwlock_init(&rwLock);
    int thread_count = 2;
    pthread_t *thread_handles = malloc(sizeof(pthread_t) * thread_count);
    for (long i = 0; i < thread_count; i++) {
        pthread_create(&thread_handles[i], NULL, func, (void *) i);
    }
    long true_counter = 0;
    long retval;
    for (long i = 0; i < thread_count; i++) {
        pthread_join(thread_handles[i], (void **) &retval);
        true_counter += retval;
    }
    printf("true counter is %ld \n", true_counter);
    printf("total counter is %ld \n", counter);
    if (true_counter == counter) {
        printf("success!!!\n");
    } else {
        printf("failed!!!\n");
    }
    rwlock_destroy(&rwLock);
}

int rwlock_init(RWLock *rwlock) {
    rwlock->wait_read_count = 0;
    rwlock->wait_write_count = 0;
    rwlock->active_read_count = 0;
    rwlock->has_active_write = false;
    pthread_cond_init(&rwlock->read, NULL);
    pthread_cond_init(&rwlock->write, NULL);
    pthread_mutex_init(&rwlock->mutex, NULL);
    return 0;
}

int rwlock_destroy(RWLock *rwlock) {
    pthread_cond_destroy(&rwlock->read);
    pthread_cond_destroy(&rwlock->write);
    pthread_mutex_destroy(&rwlock->mutex);
    return 0;
}

int rwlock_rdlock(RWLock *rwlock) {
    pthread_mutex_lock(&rwlock->mutex);
    if (rwlock->has_active_write || rwlock->wait_write_count > 0) {
        rwlock->wait_read_count++;
        while (pthread_cond_wait(&rwlock->read, &rwlock->mutex) != 0);
        rwlock->wait_read_count--;
    }
    rwlock->active_read_count++;
    pthread_mutex_unlock(&rwlock->mutex);
    return 0;
}

int rwlock_wrlock(RWLock *rwlock) {
    pthread_mutex_lock(&rwlock->mutex);
    if (rwlock->has_active_write || rwlock->wait_write_count > 0 || rwlock->active_read_count > 0) {
        rwlock->wait_write_count++;
        while (pthread_cond_wait(&rwlock->write, &rwlock->mutex) != 0);
        rwlock->wait_write_count--;
    }
    rwlock->has_active_write = true;
    pthread_mutex_unlock(&rwlock->mutex);
    return 0;
}

int rwlock_unlock(RWLock *rwlock) {
    pthread_mutex_lock(&rwlock->mutex);
    if (rwlock->has_active_write) {
        rwlock->has_active_write = false;
        if (rwlock->wait_write_count > 0) {
            pthread_cond_signal(&rwlock->write);
        } else if (rwlock->wait_read_count > 0) {
            pthread_cond_broadcast(&rwlock->read);
        }
    } else if (rwlock->active_read_count > 0) {
        rwlock->active_read_count--;
        if (rwlock->active_read_count == 0) {
            if (rwlock->wait_write_count > 0) {
                pthread_cond_signal(&rwlock->write);
            } else if (rwlock->wait_read_count > 0) {
                pthread_cond_broadcast(&rwlock->read);
            }
        }
    } else {
        perror("something error !!!");
    }
    pthread_mutex_unlock(&rwlock->mutex);
    return 0;
}
