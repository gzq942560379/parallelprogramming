#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "console_utils.h"
#include "random_uitls.h"

#define ROW_VAL 14
#define COL_VAL 12
#define START 0
#define END 10

#define ROW size[0]
#define COL size[1]

#define CROSS_BLOCK_LOW(id, p, n) ((id) * (n) / (p))
#define CROSS_BLOCK_HIGH(id, p, n) (CROSS_BLOCK_LOW((id) + 1, p, n) - 1)
#define CROSS_BLOCK_SIZE(id, p, n) \
  (CROSS_BLOCK_LOW((id) + 1, p, n) - CROSS_BLOCK_LOW(id, p, n))
#define CROSS_BLOCK_OWNER(index, p, n) (((p) * ((index) + 1) - 1) / (n))

void get_size(int *size, int rank, MPI_Comm comm);
void read_a(double *local_a, int *size, int rank, int thread_count, int local_n,
            MPI_Comm comm);
void read_b(double *local_b, int *size, int rank, int thread_count, int local_n,
            MPI_Comm comm);
void parallel_matrix_vector_multipy(double *local_a, double *local_b,
                                    double *local_c, int local_n, int *size);

int main(int argc, char **argv) {
  int size[2];
  double *local_a, *local_b;
  double *local_c;
  int local_n, local_low, local_high;
  int rank;
  int thread_count;
  MPI_Comm comm = MPI_COMM_WORLD;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(comm, &thread_count);
  MPI_Comm_rank(comm, &rank);

  get_size(size, rank, comm);
  local_n = CROSS_BLOCK_SIZE(rank, thread_count, COL);
  local_low = CROSS_BLOCK_LOW(rank, thread_count, COL);
  local_low = CROSS_BLOCK_HIGH(rank, thread_count, COL);

  local_a = malloc(ROW * local_n * sizeof(double));
  local_b = malloc(local_n * sizeof(double));
  local_c = malloc(ROW * sizeof(double));

  read_a(local_a, size, rank, thread_count, local_n, comm);
  read_b(local_b, size, rank, thread_count, local_n, comm);
  parallel_matrix_vector_multipy(local_a, local_b, local_c, local_n, size);

  if (rank == 0) {
    double *c = malloc(ROW * sizeof(double));
    MPI_Reduce(local_c, c, ROW, MPI_DOUBLE, MPI_SUM, 0, comm);
    print_double_array(c, ROW, "result");
    free(c);
  } else {
    MPI_Reduce(local_c, NULL, ROW, MPI_DOUBLE, MPI_SUM, 0, comm);
  }

  free(local_a);
  free(local_b);
  free(local_c);
  MPI_Finalize();
  exit(0);
}

void get_size(int *size, int rank, MPI_Comm comm) {
  if (rank == 0) {
    size[0] = ROW_VAL;
    size[1] = COL_VAL;
    MPI_Bcast(size, 2, MPI_INT, 0, comm);
  } else {
    MPI_Bcast(size, 2, MPI_INT, 0, comm);
  }
}

void read_a(double *local_a, int *size, int rank, int thread_count, int local_n,
            MPI_Comm comm) {
  if (rank == 0) {
    double *a = malloc(COL * sizeof(double));

    int *send_counts = malloc(thread_count * sizeof(int));
    int *displs = malloc(thread_count * sizeof(int));
    send_counts[0] = local_n;
    displs[0] = 0;
    for (int i = 1; i < thread_count; i++) {
      send_counts[i] = CROSS_BLOCK_SIZE(i, thread_count, COL);
      displs[i] = displs[i - 1] + send_counts[i - 1];
    }
    for (int i = 0; i < ROW; i++) {
      // read a row
      random_double_array(a, COL, START, END);
      MPI_Scatterv(a, send_counts, displs, MPI_DOUBLE, local_a + i * local_n,
                   local_n, MPI_DOUBLE, 0, comm);
    }
    free(a);
    free(send_counts);
    free(displs);
  } else {
    for (int i = 0; i < ROW; i++) {
      MPI_Scatterv(NULL, NULL, NULL, MPI_DOUBLE, local_a + i * local_n, local_n,
                   MPI_DOUBLE, 0, comm);
    }
  }
}

void read_b(double *local_b, int *size, int rank, int thread_count, int local_n,
            MPI_Comm comm) {
  if (rank == 0) {
    double *b = malloc(COL * sizeof(double));
    random_double_array(b, COL, START, END);

    int *send_counts = malloc(thread_count * sizeof(int));
    int *displs = malloc(thread_count * sizeof(int));
    send_counts[0] = local_n;
    displs[0] = 0;
    for (int i = 1; i < thread_count; i++) {
      send_counts[i] = CROSS_BLOCK_SIZE(i, thread_count, COL);
      displs[i] = displs[i - 1] + send_counts[i - 1];
    }
    MPI_Scatterv(b, send_counts, displs, MPI_DOUBLE, local_b, local_n,
                 MPI_DOUBLE, 0, comm);
    free(b);
    free(send_counts);
    free(displs);
  } else {
    MPI_Scatterv(NULL, NULL, NULL, MPI_DOUBLE, local_b, local_n, MPI_DOUBLE, 0,
                 comm);
  }
}

void parallel_matrix_vector_multipy(double *local_a, double *local_b,
                                    double *local_c, int local_n, int *size) {
  for (int i = 0; i < ROW; i++) {
    local_c[i] = 0;
    for (int j = 0; j < local_n; j++) {
      local_c[i] += local_a[i * COL + j] * local_b[j];
    }
  }
}