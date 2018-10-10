#include <memory.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#include "random_uitls.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))

#define CROSS_BLOCK_LOW(id, p, n) ((id) * (n) / (p))
#define CROSS_BLOCK_HIGH(id, p, n) (CROSS_BLOCK_LOW((id) + 1, p, n) - 1)
#define CROSS_BLOCK_SIZE(id, p, n) \
  (CROSS_BLOCK_LOW((id) + 1, p, n) - CROSS_BLOCK_LOW(id, p, n))
#define CROSS_BLOCK_OWNER(index, p, n) (((p) * ((index) + 1) - 1) / (n))

#define MATRIX_SET(matrix, n, row, col, val) \
  (matrix[(row) * (n) + (col)] = (val))
#define MATRIX_GET(matrix, n, row, col) matrix[(row) * (n) + (col)]

#define MAX 100000000
// #define DEBUG

void print_matrix(int* matrix, int row, int col);
void sequential_print_local_matrix(int* local_matrix, int local_n, int n,
                                   int rank, int thread_count, MPI_Comm comm);
void get_matrix(int* local_matrix, int local_n, int n, int rank,
                int thread_count, MPI_Comm comm);
void parallel_floyd(int* local_matrix, int* k_row, int rank, int thread_count,
                    int local_n, int n, int local_low, MPI_Comm comm);
void print_result(int* local_matrix, int rank, int n, int thread_count,
                  int local_n, MPI_Comm comm);

int main(int argc, char** argv) {
  int n = 1000;
  int rank;
  int thread_count;
  MPI_Comm comm = MPI_COMM_WORLD;
  int* local_matrix;
  int* k_row;
  double time, local_time;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &thread_count);

  int local_n = CROSS_BLOCK_SIZE(rank, thread_count, n);
  int local_low = CROSS_BLOCK_LOW(rank, thread_count, n);
  int local_high = CROSS_BLOCK_HIGH(rank, thread_count, n);

  local_matrix = malloc(local_n * n * sizeof(int));
  k_row = malloc(n * sizeof(int));

  get_matrix(local_matrix, local_n, n, rank, thread_count, comm);

  MPI_Barrier(comm);
  local_time = MPI_Wtime();

  parallel_floyd(local_matrix, k_row, rank, thread_count, local_n, n, local_low,
                 comm);

  local_time = MPI_Wtime() - local_time;
  MPI_Reduce(&local_time, &time, 1, MPI_DOUBLE, MPI_MAX, 0, comm);
  if (rank == 0) {
    printf("elapsed time = %e second \n", time);
    fflush(stdout);
  }

  // print_result(local_matrix, rank, n, thread_count, local_n, comm);

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

void parallel_floyd(int* local_matrix, int* k_row, int rank, int thread_count,
                    int local_n, int n, int local_low, MPI_Comm comm) {
  for (int k = 0; k < n; k++) {
    //广播 第k行
    int k_owner = CROSS_BLOCK_OWNER(k, thread_count, n);
    if (rank == k_owner) {
      int local_k = k - local_low;
      memcpy(k_row, local_matrix + local_k * n, n * sizeof(int));
      MPI_Bcast(k_row, n, MPI_INT, k_owner, comm);
    } else {
      MPI_Bcast(k_row, n, MPI_INT, k_owner, comm);
    }
    for (int i = 0; i < local_n; i++) {
      for (int j = 0; j < n; j++) {
        local_matrix[i * n + j] =
            min(local_matrix[i * n + j], local_matrix[i * n + k] + k_row[j]);
      }
    }
  }
}

void print_result(int* local_matrix, int rank, int n, int thread_count,
                  int local_n, MPI_Comm comm) {
  if (rank == 0) {
    int* matrix = malloc(n * n * sizeof(int));
    int* send_counts = malloc(thread_count * sizeof(int));
    int* send_disps = malloc(thread_count * sizeof(int));
    send_counts[0] = local_n * n;
    send_disps[0] = 0;
    for (int i = 1; i < thread_count; i++) {
      send_counts[i] = CROSS_BLOCK_SIZE(i, thread_count, n) * n;
      send_disps[i] = send_disps[i - 1] + send_counts[i - 1];
    }
    MPI_Gatherv(local_matrix, local_n * n, MPI_INT, matrix, send_counts,
                send_disps, MPI_INT, 0, comm);

    printf("print result : \n");
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        printf("%d ", matrix[i * n + j]);
      }
      printf("\n");
    }
  } else {
    MPI_Gatherv(local_matrix, local_n * n, MPI_INT, NULL, NULL, NULL, MPI_INT,
                0, comm);
  }
}