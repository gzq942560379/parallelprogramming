//
// Created by gzq on 18-7-12.
//

#ifndef PTHREAD_DEMO_THREAD_CREATE_TEST_H
#define PTHREAD_DEMO_THREAD_CREATE_TEST_H

#include <pthread.h>

int thread_count = 1000000;

pthread_t *pthread_handles;

void *func(void *rank);

#endif //PTHREAD_DEMO_THREAD_CREATE_TEST_H
