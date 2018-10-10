
add_executable(block_data_partition data_partition.c)
find_package(Threads REQUIRED)
target_link_libraries(block_data_partition Threads::Threads)
add_executable(mpi_time mpi_time.c)
add_executable(single_time single_time.c)

target_link_libraries(single_time utils)
find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})
target_link_libraries(mpi_time ${MPI_LIBRARIES})

add_executable(sequential_execution sequential_execution.c)
include_directories(${MPI_INCLUDE_PATH})
target_link_libraries(sequential_execution ${MPI_LIBRARIES})

