#include "matrix.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

void matrix_init(matrix_t *metrix_p /* out */, int row, int col) {
  metrix_p->row = row;
  metrix_p->col = col;
  metrix_p->storage = malloc(row * col * sizeof(matrix_type));
  metrix_p->metrix = malloc(row * sizeof(matrix_type *));
  for (int i = 0; i < row; i++) {
    metrix_p->metrix[i] = &(metrix_p->storage[i * col]);
  }
  memset(metrix_p->storage, 0, row * col * sizeof(matrix_type));
}
void matrix_print(matrix_t *matrix_p) {
  int row = matrix_p->row;
  int col = matrix_p->col;
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      printf("%d ", matrix_p->metrix[i][j]);
    }
    printf("\n");
  }
}
void matrix_fill(matrix_t *matrix_p, int val) {
  int row = matrix_p->row;
  int col = matrix_p->col;
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      matrix_p->metrix[i][j] = val;
    }
  }
}
void matrix_destory(matrix_t *matrix_p) {
  free(matrix_p->storage);
  free(matrix_p->metrix);
}