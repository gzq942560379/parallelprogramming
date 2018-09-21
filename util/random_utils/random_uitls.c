//
// Created by gzq on 18-7-13.
//

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "random_uitls.h"

int randint(int start, int end) {
    assert(start < end);
    return rand() % (end - start) + start;
}

double random_double() {
    return (double) rand() / ((long) (RAND_MAX) + 1);
}
