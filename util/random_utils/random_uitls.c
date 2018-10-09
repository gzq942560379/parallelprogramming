//
// Created by gzq on 18-7-13.
//

#include "random_uitls.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int randint(int start, int end) {
  assert(start < end);
  return rand() % (end - start) + start;
}

double random_double(double start, double end) {
  return ((double)rand() / ((long)(RAND_MAX) + 1)) * (end - start) + start;
}

void random_double_array(double* arr /* out */, int len, double start,
                         double end) {
  for (int i = 0; i < len; i++) {
    arr[i] = random_double(start, end);
  }
}

void random_array(int* arr /* out */, int len, int start, int end) {
  for (int i = 0; i < len; i++) {
    arr[i] = randint(start, end);
  }
}

void random_metrix(int** arr /* out */, int row, int col, int start, int end) {
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      arr[i][j] = randint(start, end);
    }
  }
}
