#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void get_input(int rank, int *n) {
    if (rank == 0) {
        printf("input n : \n");
        scanf("%d", n);
    }
    MPI_Bcast(n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("%d get %d \n", rank, n);
}

void read_vector(double *local_a, int local_n, int n, char *vector_name, int my_rank, MPI_Comm comm) {
    double *a = NULL;
    if (my_rank == 0) {
        a = malloc(n * sizeof(double));
        for (int i = 0; i < n; i++) {
            printf("input n : \n");
            scanf("%lf", &a[i]);
        }
        MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        free(a);
    } else {
        MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }
}

int main(int argc, char **argv) {
    int thread_count;
    int my_rank;
    int n;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &thread_count);

    get_input(my_rank, &n);

    double local_n = n / thread_count;
    double *local_a = malloc(sizeof(double) * local_n);
    read_vector(local_a, local_n, n, "hello", my_rank, MPI_COMM_WORLD);

    double *result;
    if (my_rank == 0) {
        result = malloc(sizeof(double) * local_n);
    }

    if (my_rank == 0) {
        double *b = malloc(sizeof(double) * n);
        MPI_Gather(local_a, local_n, MPI_DOUBLE, b, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    }

    MPI_Reduce(local_a, result, local_n, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        printf("result is:\n");
        for (int i = 0; i < local_n; i++) {
            printf("%lf ", result[i]);
        }
    }
    free(local_a);
    free(result);
    MPI_Finalize();
    exit(0);
}
