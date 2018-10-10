add_executable(floyd floyd.c)
add_executable(mpi_floyd mpi_floyd.c)

target_link_libraries(floyd utils)
target_link_libraries(floyd matrix)

target_link_libraries(mpi_floyd utils)

target_link_libraries(mpi_floyd ${MPI_LIBRARIES})
