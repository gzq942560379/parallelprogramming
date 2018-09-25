#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../util/console_utils/console_utils.h"
#include "../../util/random_utils/random_uitls.h"

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

void read_b(double *local_b, int *size, int rank, MPI_Comm comm);

void parallel_matrix_vector_multipy(double *local_c, double *local_a,
                                    double *local_b, int local_n, int *size);

void print_result(double *local_c, int *size, int rank, int thread_count,
                  int local_n, MPI_Comm comm);

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
    local_n = CROSS_BLOCK_SIZE(rank, thread_count, ROW);
    local_low = CROSS_BLOCK_LOW(rank, thread_count, ROW);
    local_low = CROSS_BLOCK_HIGH(rank, thread_count, ROW);

    local_a = malloc(local_n * COL * sizeof(double));
    local_b = malloc(COL * sizeof(double));
    local_c = malloc(local_n * sizeof(double));

    read_a(local_a, size, rank, thread_count, local_n, comm);
    read_b(local_b, size, rank, comm);
    parallel_matrix_vector_multipy(local_c, local_a, local_b, local_n, size);
    print_result(local_c, size, rank, thread_count, local_n, comm);

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
};

void read_a(double *local_a, int *size, int rank, int thread_count, int local_n,
            MPI_Comm comm) {
    if (rank == 0) {
        double *a = malloc(ROW * COL * sizeof(double));
        random_double_array(a, ROW * COL, START, END);

        int *send_counts = malloc(thread_count * sizeof(int));
        int *send_disps = malloc(thread_count * sizeof(int));
        send_counts[0] = local_n * COL;
        send_disps[0] = 0;
        for (int i = 1; i < thread_count; i++) {
            send_counts[i] = CROSS_BLOCK_SIZE(i, thread_count, ROW) * COL;
            send_disps[i] = send_disps[i - 1] + send_counts[i - 1];
        }
        MPI_Scatterv(a, send_counts, send_disps, MPI_DOUBLE, local_a, local_n * COL,
                     MPI_DOUBLE, 0, comm);

        free(a);
        free(send_counts);
        free(send_disps);
    } else {
        MPI_Scatterv(NULL, NULL, NULL, MPI_DOUBLE, local_a, local_n * COL,
                     MPI_DOUBLE, 0, comm);
    }
}

void read_b(double *local_b, int *size, int rank, MPI_Comm comm) {
    if (rank == 0) {
        random_double_array(local_b, COL, START, END);
        MPI_Bcast(local_b, COL, MPI_DOUBLE, 0, comm);
    } else {
        MPI_Bcast(local_b, COL, MPI_DOUBLE, 0, comm);
    }
}

void parallel_matrix_vector_multipy(double *local_c, double *local_a,
                                    double *local_b, int local_n, int *size) {
    for (int i = 0; i < local_n; i++) {
        local_c[i] = 0;
        for (int j = 0; j < COL; j++) {
            local_c[i] += local_a[i * COL + j] * local_b[j];
        }
    }
}

void print_result(double *local_c, int *size, int rank, int thread_count,
                  int local_n, MPI_Comm comm) {
    if (rank == 0) {
        double *c = malloc(ROW * sizeof(double));
        int *recvcounts = malloc(thread_count * sizeof(int));
        int *displs = malloc(thread_count * sizeof(int));
        recvcounts[0] = local_n;
        displs[0] = 0;
        for (int i = 1; i < thread_count; i++) {
            recvcounts[i] = CROSS_BLOCK_SIZE(i, thread_count, ROW);
            displs[i] = displs[i - 1] + recvcounts[i - 1];
        }
        MPI_Gatherv(local_c, local_n, MPI_DOUBLE, c, recvcounts, displs, MPI_DOUBLE,
                    0, comm);
        print_double_array(c, ROW, "result");
        free(c);
        free(recvcounts);
        free(displs);
    } else {
        MPI_Gatherv(local_c, local_n, MPI_DOUBLE, NULL, NULL, NULL, MPI_DOUBLE, 0,
                    comm);
    }
}