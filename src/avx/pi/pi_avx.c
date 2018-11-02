//
// Created by gzq on 18-10-9.
//

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>
#include "time_utils.h"


void main(int argc, char **argv) {

    timer timer;
    start(&timer);
    size_t dt = 100000000;
    double pi = 0.0;
    double delta = 1.0 / dt;
    __m256d pi_4, one_4, delta_4, x_4, x_h_4;

    one_4 = _mm256_set1_pd(1.0);
    delta_4 = _mm256_set1_pd(delta);
    pi_4 = _mm256_setzero_pd();
    x_h_4 = _mm256_set_pd(delta * 3, delta * 2, delta, 0.0);

    for (int i = 0; i < dt; i += 4) {
        x_4 = _mm256_set1_pd(i * delta);
        x_4 = _mm256_add_pd(x_4, x_h_4);
        x_4 = _mm256_mul_pd(x_4, x_4);
        x_4 = _mm256_add_pd(one_4, x_4);
        x_4 = _mm256_div_pd(delta_4, x_4);
        pi_4 = _mm256_add_pd(pi_4, x_4);
    }
    double tmp[4]__attribute__((aligned(32)));
    _mm256_store_pd(tmp,pi_4);
    pi += tmp[0]+tmp[1]+tmp[2]+tmp[3];
    pi *= 4.0;
    end(&timer);
    print_milli_interval(&timer);

    printf("pi = %.50lf\n", pi);
}