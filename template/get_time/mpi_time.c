#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv) {
  double time, local_time;
  int rank;
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(comm, &rank);
  MPI_Barrier(comm);
  local_time = MPI_Wtime();
  // TODO

  local_time = MPI_Wtime() - local_time;
  MPI_Reduce(&local_time, &time, 1, MPI_DOUBLE, MPI_MAX, 0, comm);
  if (rank == 0) {
    printf("elapsed time = %e second \n", time);
  }
  MPI_Finalize();
  exit(0);
}