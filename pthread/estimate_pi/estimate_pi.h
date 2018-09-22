//
// Created by gzq on 18-7-12.
//

#ifndef PTHREAD_DEMO_ESTIMATE_PI_H

#include <pthread.h>
#include "stdio.h"
#include "stdlib.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))

int seed;
double rand_double(double a, double b);

long long total_count = 100000000;
int thread_count = 1;
long long total_hit_number = 0;
pthread_mutex_t *mutex;

#define PTHREAD_DEMO_ESTIMATE_PI_H

#endif  // PTHREAD_DEMO_ESTIMATE_PI_H
