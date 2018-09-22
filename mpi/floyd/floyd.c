#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../collections/matrix/matrix.h"
#include "../../util/random_utils/random_uitls.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define MAX 100000000

int main(int argc, char **argv) {
  matrix_t matrix;
  int n = 5;
  matrix_init(&matrix, n, n);

  matrix_fill(&matrix, MAX);
  matrix.metrix[0][1] = 1;
  matrix.metrix[0][2] = 3;
  matrix.metrix[0][3] = 5;
  matrix.metrix[0][4] = 20;
  matrix.metrix[1][2] = 1;
  matrix.metrix[1][4] = 2;
  matrix.metrix[2][3] = 1;
  matrix.metrix[3][4] = 1;
  matrix_print(&matrix);

  matrix_t path_matrix;
  matrix_init(&path_matrix, n, n);
  matrix_fill(&path_matrix, -1);

  for (int k = 0; k < n; k++) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (matrix.metrix[i][j] > matrix.metrix[i][k] + matrix.metrix[k][j]) {
          matrix.metrix[i][j] = matrix.metrix[i][k] + matrix.metrix[k][j];
          path_matrix.metrix[i][j] = k;
        }
      }
    }
  }
  printf("\n");
  matrix_print(&matrix);
  printf("\n");
  matrix_print(&path_matrix);

  matrix_destory(&matrix);
  matrix_destory(&path_matrix);
  exit(0);
}
