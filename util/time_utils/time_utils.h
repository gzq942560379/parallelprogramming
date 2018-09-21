//
// Created by gzq on 18-7-12.
//

#include <stdbool.h>

#ifndef PTHREAD_DEMO_TIME_UTILS_H
#define PTHREAD_DEMO_TIME_UTILS_H


struct timeval t1;
struct timeval t2;

long sec(struct timeval *tv);

long msec(struct timeval *tv);

long usec(struct timeval *tv);

void click_s();

void click_e();

long second_interval();
long milli_interval();
long micro_interval();

void print_second_interval();

void print_milli_interval();

void print_micro_interval();




#endif //PTHREAD_DEMO_TIME_UTILS_H
