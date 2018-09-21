#ifndef check_error_h
#define check_error_h
#include <mpi.h>
void check_for_error(int local_ok, MPI_Comm comm, char* fname,
                     const char* message, ...);
#endif  // check_error_h