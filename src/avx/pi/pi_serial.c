//
// Created by gzq on 18-10-9.
//



#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "time_utils.h"

void main(int argc, char **argv) {

    timer timer;
    start(&timer);
    size_t dt = 100000000;
    double pi = 0.0;
    double delta = 1.0 / dt;
    double x;
    for (size_t i = 0; i < dt; i++) {
        x = i * delta;
        pi += delta / (1 + x * x);
    }
    pi *= 4;
    end(&timer);
    print_milli_interval(&timer);

    printf("pi = %.50lf\n", pi);
}