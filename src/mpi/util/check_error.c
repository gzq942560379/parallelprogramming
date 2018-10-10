#include "check_error.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void check_for_error(int local_ok, MPI_Comm comm, char* fname,
                     const char* message, ...) {
  int ok = 0;
  MPI_Allreduce(&local_ok, &ok, 1, MPI_INT, MPI_MIN, comm);
  if (ok == 0) {
    int rank = 0;
    MPI_Comm_rank(comm, &rank);
    if (rank == 0) {
      fprintf(stderr, "Proc %d > In %s , ", rank, fname);
      va_list args;
      va_start(args, message);
      vfprintf(stderr, message, args);
      va_end(args);
      fprintf(stderr, "\n");
      fflush(stderr);
    }
    MPI_Finalize();
    exit(-1);
  }
}