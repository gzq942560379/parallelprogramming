#ifndef matrix_h
#define matrix_h

typedef int matrix_type;

typedef struct {
  matrix_type **metrix;
  matrix_type *storage;
  int row;
  int col;
} matrix_t;

void matrix_init(matrix_t *metrix_p /* out */, int row, int col);

void matrix_print(matrix_t *matrix_p);

void matrix_fill(matrix_t *matrix_p, int val);

void matrix_destory(matrix_t *matrix_p);

#endif  // matrix_h