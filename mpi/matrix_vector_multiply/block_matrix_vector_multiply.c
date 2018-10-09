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
#define ROW_DIM 0
#define COL_DIM 1

#define CROSS_BLOCK_LOW(id, p, n) ((id) * (n) / (p))
#define CROSS_BLOCK_HIGH(id, p, n) (CROSS_BLOCK_LOW((id) + 1, p, n) - 1)
#define CROSS_BLOCK_SIZE(id, p, n) \
  (CROSS_BLOCK_LOW((id) + 1, p, n) - CROSS_BLOCK_LOW(id, p, n))
#define CROSS_BLOCK_OWNER(index, p, n) (((p) * ((index) + 1) - 1) / (n))

#define A 0
#define B 0


void get_size(int *size, int comm_rank, MPI_Comm comm);

void read_a(double *local_a, int local_row, int local_col, int *size,
            int cart_rank, int *coords, int *dim_size, MPI_Comm cart_comm,
            MPI_Comm row_comm);

void
read_b(MPI_Comm row_comm, MPI_Comm col_comm, int cart_rank, const int *dim_size, const int *size, const int *coords,
       int local_col, double *local_b);

int main(int argc, char **argv) {
    MPI_Comm comm = MPI_COMM_WORLD, cart_comm, row_comm, col_comm;
    int comm_rank, thread_count, cart_rank, row_rank, col_rank;
    int dims = 2;
    int dim_size[2] = {2, 2};
    int periods[2] = {0, 0};

    int size[2];
    int coords[2];

    int local_row, local_col;
    double *local_a, *local_b, *local_c, *reduce_local_c;

    MPI_Init(&argc, &argv);
    // 全局通信域
    MPI_Comm_rank(comm, &comm_rank);
    MPI_Comm_size(comm, &thread_count);
    // 笛卡尔拓扑
    MPI_Cart_create(comm, dims, dim_size, periods, 1, &cart_comm);
    MPI_Comm_rank(cart_comm, &cart_rank);
    MPI_Cart_coords(cart_comm, cart_rank, dims, coords);
    // 行通信域
    MPI_Comm_split(cart_comm, coords[ROW_DIM], coords[COL_DIM], &row_comm);
    MPI_Comm_rank(row_comm, &row_rank);
    // 列通信域
    MPI_Comm_split(cart_comm, coords[COL_DIM], coords[ROW_DIM], &col_comm);
    MPI_Comm_rank(row_comm, &col_rank);

    get_size(size, comm_rank, comm);
    local_row = CROSS_BLOCK_SIZE(coords[ROW_DIM], dim_size[ROW_DIM], ROW);
    local_col = CROSS_BLOCK_SIZE(coords[COL_DIM], dim_size[COL_DIM], COL);

    local_a = malloc(local_row * local_col * sizeof(double));
    local_b = malloc(local_col * sizeof(double));
    local_c = malloc(local_row * sizeof(double));

    read_a(local_a, local_row, local_col, size, cart_rank, coords, dim_size,
           cart_comm, row_comm);
    read_b(row_comm, col_comm, cart_rank, dim_size, size, coords, local_col, local_b);

    // parallel compute
    for (int i = 0; i < local_row; i++) {
        local_c[i] = 0;
        for (int j = 0; j < local_col; j++) {
            local_c[i] += local_a[i * local_col + j] * local_b[j];
        }
    }
    // reduce and gather
    if (coords[COL_DIM] == 0) {
        reduce_local_c = malloc(local_row * sizeof(double));
        MPI_Reduce(local_c, reduce_local_c, local_row, MPI_DOUBLE, MPI_SUM, 0, row_comm);
    } else {
        MPI_Reduce(local_c, NULL, local_row, MPI_DOUBLE, MPI_SUM, 0, row_comm);
    }
    if (coords[COL_DIM] == 0) {
        if (coords[ROW_DIM] == 0) {
            double *c = malloc(ROW * sizeof(double));
            int *recvcounts = malloc(dim_size[COL_DIM] * sizeof(int));
            int *displs = malloc(dim_size[COL_DIM] * sizeof(int));
            recvcounts[0] = CROSS_BLOCK_SIZE(0, dim_size[COL_DIM], ROW);
            displs[0] = 0;
            for (int i = 1; i < dim_size[COL_DIM]; ++i) {
                recvcounts[i] = CROSS_BLOCK_SIZE(i, dim_size[COL_DIM], ROW);
                displs[i] = displs[i - 1] + recvcounts[i - 1];
            }
            MPI_Gatherv(reduce_local_c, local_row, MPI_DOUBLE, c, recvcounts, displs, MPI_DOUBLE, 0, col_comm);
            print_double_array(c, ROW, "result");
            free(c);
            free(recvcounts);
            free(displs);
        } else {
            MPI_Gatherv(reduce_local_c, local_row, MPI_DOUBLE, NULL, NULL, NULL, MPI_DOUBLE, 0, col_comm);
        }
    }

    MPI_Finalize();
    exit(0);
}


void get_size(int *size, int comm_rank, MPI_Comm comm) {
    if (comm_rank == 0) {
        size[0] = ROW_VAL;
        size[1] = COL_VAL;
        MPI_Bcast(size, 2, MPI_INT, 0, comm);
    } else {
        MPI_Bcast(size, 2, MPI_INT, 0, comm);
    }
}

void read_a(double *local_a, int local_row, int local_col, int *size,
            int cart_rank, int *coords, int *dim_size, MPI_Comm cart_comm,
            MPI_Comm row_comm) {
    int *send_counts = malloc(dim_size[ROW_DIM] * sizeof(int));
    int *displs = malloc(dim_size[ROW_DIM] * sizeof(int));
    send_counts[0] = CROSS_BLOCK_SIZE(0, dim_size[ROW_DIM], COL);
    displs[0] = 0;
    for (int i = 1; i < dim_size[ROW_DIM]; i++) {
        send_counts[i] = CROSS_BLOCK_SIZE(i, dim_size[ROW_DIM], COL);
        displs[i] = displs[i - 1] + send_counts[i - 1];
    }
    if (cart_rank == 0) {
        int dest_rank;
        int dest_coords[2];
        dest_coords[1] = 0;
        double *a = malloc(COL * sizeof(double));
        for (int i = 0; i < ROW; i++) {
            dest_coords[0] = CROSS_BLOCK_OWNER(i, dim_size[ROW_DIM], ROW);
            MPI_Cart_rank(cart_comm, dest_coords, &dest_rank);
            random_double_array(a, COL, START, END);
            if (cart_rank != dest_rank) {
                MPI_Send(a, COL, MPI_DOUBLE, dest_rank, A, cart_comm);
            } else {
                MPI_Scatterv(a, send_counts, displs, MPI_DOUBLE,
                             local_a + i * local_col, local_col, MPI_DOUBLE, 0,
                             row_comm);
            }
        }
        free(a);
    } else if (coords[COL_DIM] == 0) {
        double *a = malloc(COL * sizeof(double));
        for (int i = 0; i < local_row; i++) {
            MPI_Recv(a, COL, MPI_DOUBLE, 0, A, cart_comm, NULL);
            MPI_Scatterv(a, send_counts, displs, MPI_DOUBLE, local_a + i * local_col,
                         local_col, MPI_DOUBLE, 0, row_comm);
        }
        free(a);
    } else {
        for (int i = 0; i < local_row; i++) {
            MPI_Scatterv(NULL, NULL, NULL, MPI_DOUBLE, local_a + i * local_col,
                         local_col, MPI_DOUBLE, 0, row_comm);
        }
    }
    free(send_counts);
    free(displs);
}

void
read_b(MPI_Comm row_comm, MPI_Comm col_comm, int cart_rank, const int *dim_size, const int *size, const int *coords,
       int local_col, double *local_b) {
    if (cart_rank == 0) {
        int *send_counts = malloc(dim_size[ROW_DIM] * sizeof(int));
        int *displs = malloc(dim_size[ROW_DIM] * sizeof(int));
        send_counts[0] = CROSS_BLOCK_SIZE(0, dim_size[ROW_DIM], COL);
        displs[0] = 0;
        for (int i = 1; i < dim_size[ROW_DIM]; i++) {
            send_counts[i] = CROSS_BLOCK_SIZE(i, dim_size[ROW_DIM], COL);
            displs[i] = displs[i - 1] + send_counts[i - 1];
        }
        double *b = malloc(COL * sizeof(double));
        random_double_array(b, COL, START, END);
        MPI_Scatterv(b, send_counts, displs, MPI_DOUBLE, local_b, local_col, MPI_DOUBLE, 0, row_comm);
    } else if (coords[ROW_DIM] == 0) {
        MPI_Scatterv(NULL, NULL, NULL, MPI_DOUBLE, local_b, local_col, MPI_DOUBLE, 0, row_comm);
    }
    if (coords[ROW_DIM] == 0) {
        MPI_Bcast(local_b, local_col, MPI_DOUBLE, 0, col_comm);
    } else {
        MPI_Bcast(local_b, local_col, MPI_DOUBLE, 0, col_comm);
    }
}
