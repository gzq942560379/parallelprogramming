#include <memory.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../collections/matrix/matrix.h"
#include "../../util/random_utils/random_uitls.h"

#define CROSS_BLOCK_LOW(id, p, n) ((id) * (n) / (p))
#define CROSS_BLOCK_HIGH(id, p, n) (CROSS_BLOCK_LOW((id) + 1, p, n) - 1)
#define CROSS_BLOCK_SIZE(id, p, n) \
  (CROSS_BLOCK_LOW((id) + 1, p, n) - CROSS_BLOCK_LOW(id, p, n))
#define CROSS_BLOCK_OWNER(index, p, n) (((p) * ((index) + 1) - 1) / (n))

#define MATRIX_SET(matrix, n, row, col, val) \
  (matrix[(row) * (n) + (col)] = (val))
#define MATRIX_GET(matrix, n, row, col) matrix[(row) * (n) + (col)]
#define MAX 100000000

void print_matrix(int* matrix, int row, int col);
void sequential_print_local_matrix(int* local_matrix, int local_n, int n,
                                   int rank, int thread_count, MPI_Comm comm);
void get_matrix(int* local_matrix, int local_n, int n, int rank,
                int thread_count, MPI_Comm comm);

int main(int argc, char** argv) {
  int n = 14;
  int rank;
  int thread_count;
  MPI_Comm comm = MPI_COMM_WORLD;
  int* local_matrix;
  int* k_row;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &thread_count);

  int local_n = CROSS_BLOCK_SIZE(rank, thread_count, n);
  int local_low = CROSS_BLOCK_LOW(rank, thread_count, n);
  int local_high = CROSS_BLOCK_HIGH(rank, thread_count, n);

  local_matrix = malloc(local_n * n * sizeof(int));
  k_row = malloc(n * sizeof(int));
#ifdef DEBUG
  printf("rank %d ,local_n = %d,local_low = %d,local_high = %d\n", rank,
         local_n, local_low, local_high);
#endif

  get_matrix(local_matrix, local_n, n, rank, thread_count, comm);
#ifdef DEBUG
  sequential_print_local_matrix(local_matrix, local_n, n, rank, thread_count,
                                comm);
#endif
  for (int k = 0; k < n; k++) {
    //广播 第k行
    int k_owner = CROSS_BLOCK_OWNER(k, thread_count, n);
  }
  free(k_row);
  free(local_matrix);
  MPI_Finalize();
  exit(0);
}

void print_matrix(int* matrix, int row, int col) {
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      printf("%d ", MATRIX_GET(matrix, col, i, j));
    }
    printf("\n");
  }
}

void sequential_print_local_matrix(int* local_matrix, int local_n, int n,
                                   int rank, int thread_count, MPI_Comm comm) {
  if (rank == 0) {
    int ok = 1;
    printf("In %d :\n", rank);
    print_matrix(local_matrix, local_n, n);
    printf("\n");
    MPI_Send(&ok, 1, MPI_INT, rank + 1, 0, comm);
  } else {
    int ok;
    MPI_Recv(&ok, 1, MPI_INT, rank - 1, 0, comm, NULL);
    printf("In %d :\n", rank);
    print_matrix(local_matrix, local_n, n);
    printf("\n");
    if (rank != thread_count - 1) {
      MPI_Send(&ok, 1, MPI_INT, rank + 1, 0, comm);
    }
  }
}

void get_matrix(int* local_matrix, int local_n, int n, int rank,
                int thread_count, MPI_Comm comm) {
  if (rank == 0) {
    // init matrix
    int* matrix = malloc(n * n * sizeof(int));
    random_array(matrix, n * n, 0, 100);
    for (int i = 0; i < n; i++) {
      MATRIX_SET(matrix, n, i, i, 0);
    }
#ifdef DEBUG
    printf("random init matrix :\b");
    print_matrix(matrix, n, n);
    printf("\n");
#endif
    int* send_counts = malloc(thread_count * sizeof(int));
    int* send_disps = malloc(thread_count * sizeof(int));
    send_counts[0] = local_n * n;
    send_disps[0] = 0;
    for (int i = 1; i < thread_count; i++) {
      send_counts[i] = CROSS_BLOCK_SIZE(i, thread_count, n) * n;
      send_disps[i] = send_disps[i - 1] + send_counts[i - 1];
    }
    MPI_Scatterv(matrix, send_counts, send_disps, MPI_INT, local_matrix,
                 local_n * n, MPI_INT, 0, comm);

    free(matrix);
  } else {
    MPI_Scatterv(NULL, NULL, NULL, MPI_INT, local_matrix, local_n * n, MPI_INT,
                 0, comm);
  }
}