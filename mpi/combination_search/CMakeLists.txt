
add_executable(mpi_backtrack_permutation mpi_backtrack_permutation.c)


find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})
target_link_libraries(mpi_backtrack_permutation ${MPI_LIBRARIES})

target_link_libraries(mpi_backtrack_permutation console_utils)