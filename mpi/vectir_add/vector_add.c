#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void check_for_error(int local_ok, char* fname, char* message, MPI_Comm comm);
void get_n(int* n_p, int rank, int thread_count, MPI_Comm comm);
void allocate_vectors(double** local_a_p, double** local_b_p,
                      double** local_c_p, int local_n, MPI_Comm comm);
void deallocate_vectors(double* local_a_p, double* local_b_p,
                        double* local_c_p);
void read_vector(double* local_a, int local_n, int n, int rank,
                 char* vector_name, MPI_Comm comm);
void print_vector(double* local_a, int local_n, int n, int rank,
                  char* vector_name, MPI_Comm comm);

void parallel_vector_sum(double* local_a, double* local_b, double* local_c,
                         int local_n);
int main(int argc, char** argv) {
  MPI_Comm comm = MPI_COMM_WORLD;
  int thread_count;
  int rank;
  int n;
  double *local_a, *local_b, *local_c;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &thread_count);

  get_n(&n, rank, thread_count, comm);

  int local_n = n / thread_count;
  allocate_vectors(&local_a, &local_b, &local_c, local_n, comm);

  read_vector(local_a, local_n, n, rank, "a", comm);
  print_vector(local_a, local_n, n, rank, "a", comm);
  read_vector(local_b, local_n, n, rank, "b", comm);
  print_vector(local_b, local_n, n, rank, "b", comm);

  parallel_vector_sum(local_a, local_b, local_c, local_n);
  print_vector(local_c, local_n, n, rank, "c", comm);

  deallocate_vectors(local_a, local_b, local_c);
  MPI_Finalize();
  exit(0);
}

void get_n(int* n_p, int rank, int thread_count, MPI_Comm comm) {
  int local_ok = 1;
  if (rank == 0) {
    printf("input n:\n");
    scanf("%d", n_p);
  }
  MPI_Bcast(n_p, 1, MPI_INT, 0, comm);
  if (*n_p <= 0 || *n_p % thread_count != 0) local_ok = 0;
  check_for_error(local_ok, "get_n",
                  "n should be > 0 and evenly divisible by comm_sz", comm);
}

void read_vector(double* local_a, int local_n, int n, int rank,
                 char* vector_name, MPI_Comm comm) {
  double* a = NULL;
  int local_ok = 1;
  char* fname = "read_vector";
  if (rank == 0) {
    printf("read vector %s :\n", vector_name);
    a = (double*)malloc(n * sizeof(double));
    if (a == NULL) local_ok = 0;
    check_for_error(local_ok, fname, "Can't allocate temporary vector", comm);
    for (int i = 0; i < n; i++) {
      scanf("%lf", &a[i]);
    }
    MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0, comm);
    free(a);
  } else {
    check_for_error(local_ok, fname, "Can't allocate temporary vector", comm);
    MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0, comm);
  }
}

void print_vector(double* local_a, int local_n, int n, int rank,
                  char* vector_name, MPI_Comm comm) {
  double* b = NULL;
  char* fname = "print_vector";
  int local_ok = 1;
  if (rank == 0) {
    b = (double*)malloc(sizeof(double) * n);
    if (b == NULL) local_ok = 0;
    check_for_error(local_ok, fname, "Can't allocate temporary vector", comm);
    MPI_Gather(local_a, local_n, MPI_DOUBLE, b, local_n, MPI_DOUBLE, 0, comm);
    printf("print vector %s : \n", vector_name);
    for (int i = 0; i < n; i++) {
      printf("%lf ", b[i]);
    }
    printf("\n");
    free(b);
  } else {
    check_for_error(local_ok, fname, "Can't allocate temporary vector", comm);
    MPI_Gather(local_a, local_n, MPI_DOUBLE, b, local_n, MPI_DOUBLE, 0, comm);
  }
}

void allocate_vectors(double** local_a_p, double** local_b_p,
                      double** local_c_p, int local_n, MPI_Comm comm) {
  int local_ok = 0;
  char* fname = "allocate_vectors";
  *local_a_p = malloc(local_n * sizeof(double));
  *local_b_p = malloc(local_n * sizeof(double));
  *local_c_p = malloc(local_n * sizeof(double));
  if (*local_a_p == NULL || *local_b_p == NULL || *local_c_p == NULL)
    local_ok = 0;
  check_for_error(local_ok, fname, "Can't allocate local vector(s)", comm);
}
void deallocate_vectors(double* local_a, double* local_b, double* local_c) {
  free(local_a);
  free(local_b);
  free(local_c);
}

void parallel_vector_sum(double* local_a, double* local_b, double* local_c,
                         int local_n) {
  for (int i = 0; i < local_n; i++) {
    local_c[i] = local_a[i] + local_b[i];
  }
}
void check_for_error(int local_ok, char* fname, char* message, MPI_Comm comm) {
  int ok;
  MPI_Allreduce(&local_ok, &ok, 1, MPI_INT, MPI_MIN, comm);
  if (ok == 0) {
    int rank;
    MPI_Comm_rank(comm, &rank);
    if (rank == 0) {
      fprintf(stderr, "Proc %d > In %s , %s\n", rank, fname, message);
      fflush(stderr);
    }
    MPI_Finalize();
    exit(-1);
  }
}