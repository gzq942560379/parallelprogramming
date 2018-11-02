//
// Created by gzq on 18-7-12.
//

#include "time_utils.h"
#include <stdio.h>
#include <sys/time.h>

void start(timer *timer_p) { gettimeofday(&timer_p->t1, NULL); }

void end(timer *timer_p) { gettimeofday(&timer_p->t2, NULL); }

long sec(struct timeval *tv) { return tv->tv_sec; }

long msec(struct timeval *tv) { return tv->tv_sec * 1000 + tv->tv_usec / 1000; }

long usec(struct timeval *tv) { return tv->tv_sec * 1000000 + tv->tv_usec; }

void print_second_interval(timer *timer_p) {
  printf("cost time %ld s \n", sec(&timer_p->t2) - sec(&timer_p->t1));
}

void print_milli_interval(timer *timer_p) {
  printf("cost time %ld ms \n", msec(&timer_p->t2) - msec(&timer_p->t1));
}

void print_micro_interval(timer *timer_p) {
  printf("cost time %ld us \n", usec(&timer_p->t2) - usec(&timer_p->t1));
}

long second_interval(timer *timer_p) { return sec(&timer_p->t2) - sec(&timer_p->t1); }

long milli_interval(timer *timer_p) { return msec(&timer_p->t2) - msec(&timer_p->t1); }

long micro_interval(timer *timer_p) { return usec(&timer_p->t2) - usec(&timer_p->t1); }