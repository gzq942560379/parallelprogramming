add_executable(vector_add vector_add.c)

target_link_libraries(vector_add ${MPI_LIBRARIES})

target_link_libraries(vector_add mpi_utils)