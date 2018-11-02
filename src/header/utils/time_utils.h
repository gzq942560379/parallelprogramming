//
// Created by gzq on 18-7-12.
//

#include <stdbool.h>
#include <sys/time.h>


#ifndef PTHREAD_DEMO_TIME_UTILS_H
#define PTHREAD_DEMO_TIME_UTILS_H


/* The argument now should be a double (not a pointer to a double) */
#define GET_TIME(now)                           \
    {                                           \
        struct timeval t;                       \
        gettimeofday(&t, NULL);                 \
        now = t.tv_sec + t.tv_usec / 1000000.0; \
    }
typedef struct {
    struct timeval t1;
    struct timeval t2;
} timer;

long sec(struct timeval *tv);

long msec(struct timeval *tv);

long usec(struct timeval *tv);

void start(timer *timer_p);

void end(timer *timer_p);

long second_interval(timer *timer_p);

long milli_interval(timer *timer_p);

long micro_interval(timer *timer_p);

void print_second_interval(timer *timer_p);

void print_milli_interval(timer *timer_p);

void print_micro_interval(timer *timer_p);


#endif //PTHREAD_DEMO_TIME_UTILS_H