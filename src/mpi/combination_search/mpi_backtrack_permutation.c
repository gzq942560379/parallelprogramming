//
// Created by gzq on 18-9-25.
//

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <stdbool.h>
#include <memory.h>

#include "console_utils.h"

void parallel_backtrack(int *temp_buffer, int level);

bool check_solution(int *temp_buffer);

bool check_forward(int step, int *temp_buffer, int level);

void forward(int step, int *temp_buffer, int level);

void backtrack(int *temp_buffer);

int n;
int result_counter;
int *result_buffer;
int rank, thread_count;
int factor_n = 1;
int cutoff_depth = -1;
int cutoff_count = 0;
MPI_Comm comm = MPI_COMM_WORLD;
int depth;

int main(int argc, char **argv) {
    n = 4;
    depth = n;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(comm, &thread_count);
    MPI_Comm_rank(comm, &rank);
    for (int i = n; i >= 1; i--) {
        factor_n *= i;
        if (cutoff_depth == -1 && factor_n % thread_count == 0) {
            cutoff_depth = n - i + 1;
        }
    }
    if (cutoff_depth == -1) {
        fprintf(stderr, "can not split tree !!!");
        MPI_Finalize();
        exit(-1);
    }
    // 保存结果
    result_counter = 0;
    result_buffer = malloc(n * factor_n * sizeof(int) / thread_count);
    // 临时状态
    int level = 0;
    int *temp_buffer = malloc(n * sizeof(int));

    parallel_backtrack(temp_buffer, level);

    if (rank == 0) {
        int ok = 1;

        printf("in %d \n", rank);
        for (int i = 0; i < result_counter; ++i) {
            print_int_array(result_buffer + i * n, n, NULL);
        }

        MPI_Send(&ok, 1, MPI_INT, rank + 1, 0, comm);
    } else {
        int ok;
        MPI_Recv(&ok, 1, MPI_INT, rank - 1, 0, comm, NULL);
        printf("in %d \n", rank);
        for (int i = 0; i < result_counter; ++i) {
            print_int_array(result_buffer + i * n, n, NULL);
        }
        if (rank != thread_count - 1) {
            MPI_Send(&ok, 1, MPI_INT, rank + 1, 0, comm);
        }
    }

    MPI_Finalize();
    exit(0);
}

void parallel_backtrack(int *temp_buffer, int level) {
    if (level == depth) {
        if (check_solution(temp_buffer)) {
            memcpy(result_buffer + result_counter * n, temp_buffer, n * sizeof(int));
            result_counter++;
        }
    } else {
        if (level == cutoff_depth) {
            cutoff_count++;
            if ((cutoff_count - 1) % thread_count != rank) {
                return;
            }
        }
        for (int i = 0; i < n; ++i) {
            if (!check_forward(i, temp_buffer, level)) continue;
            forward(i, temp_buffer, level);
            parallel_backtrack(temp_buffer, level + 1);
            backtrack(temp_buffer);
        }
    }
}

void backtrack(int *temp_buffer) {
    // do nothing
}

void forward(int step, int *temp_buffer, int level) {
    temp_buffer[level] = step;
}

bool check_forward(int step, int *temp_buffer, int level) {
    for (int i = 0; i < level; ++i) {
        if (temp_buffer[i] == step) {
            return false;
        }
    }
    return true;
}

bool check_solution(int *temp_buffer) {
    return true;
}

