aux_source_directory(. MPI_UTILS_SOURCE)

add_library(mpi_utils ${MPI_UTILS_SOURCE})

target_link_libraries(mpi_utils ${MPI_LIBRARIES})