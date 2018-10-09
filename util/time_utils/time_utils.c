//
// Created by gzq on 18-7-12.
//

#include "time_utils.h"
#include <stdio.h>
#include <sys/time.h>

void click_s() { gettimeofday(&t1, NULL); }

void click_e() { gettimeofday(&t2, NULL); }

long sec(struct timeval *tv) { return tv->tv_sec; }

long msec(struct timeval *tv) { return tv->tv_sec * 1000 + tv->tv_usec / 1000; }

long usec(struct timeval *tv) { return tv->tv_sec * 1000000 + tv->tv_usec; }

void print_second_interval() {
  printf("cost time %ld s \n", sec(&t2) - sec(&t1));
}

void print_milli_interval() {
  printf("cost time %ld ms \n", msec(&t2) - msec(&t1));
}

void print_micro_interval() {
  printf("cost time %ld us \n", usec(&t2) - usec(&t1));
}

long second_interval() { return sec(&t2) - sec(&t1); }

long milli_interval() { return msec(&t2) - msec(&t1); }

long micro_interval() { return usec(&t2) - usec(&t1); }
