
add_executable(floyd floyd.c)
add_executable(mpi_floyd mpi_floyd.c)

target_link_libraries(floyd random_uitls)
target_link_libraries(floyd time_utils)
target_link_libraries(floyd matrix)

target_link_libraries(mpi_floyd random_uitls)
target_link_libraries(mpi_floyd matrix)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})
target_link_libraries(mpi_floyd ${MPI_LIBRARIES})
