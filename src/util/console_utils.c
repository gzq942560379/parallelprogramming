#include "console_utils.h"
#include <stdio.h>

void println() { printf("\n"); }

void print_int_array(int* arr, int len, char* arrar_name) {
  if (arrar_name != NULL) {
    printf("%s : \n", arrar_name);
  }
  for (int i = 0; i < len; i++) {
    printf("%d ", arr[i]);
  }
  println();
}

void print_double_array(double* arr, int len, char* arrar_name) {
  if (arrar_name != NULL) {
    printf("%s : \n", arrar_name);
  }
  for (int i = 0; i < len; i++) {
    printf("%lf ", arr[i]);
  }
  println();
}

void print_double_matrix(double* arr, int row, int col, char* matrix_name) {
  if (matrix_name != NULL) {
    printf("%s : \n", matrix_name);
  }
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      printf("%lf ", arr[i * col + j]);
    }
    println();
  }
}