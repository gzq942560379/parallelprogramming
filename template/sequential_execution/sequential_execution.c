#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  int thread_count;
  int rank;

  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(comm, &thread_count);
  MPI_Comm_rank(comm, &rank);

  if (rank == 0) {
    int ok = 1;
    printf("hello world , i am %d \n", rank);
    MPI_Send(&ok, 1, MPI_INT, rank + 1, 0, comm);
  } else {
    int ok;
    MPI_Recv(&ok, 1, MPI_INT, rank - 1, 0, comm, NULL);
    printf("hello world , i am %d \n", rank);
    if (rank != thread_count - 1) {
      MPI_Send(&ok, 1, MPI_INT, rank + 1, 0, comm);
    }
  }

  MPI_Finalize();
  exit(0);
}