add_executable(mpi_time mpi_time.c)
add_executable(single_time single_time.c)

target_link_libraries(single_time time_utils)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})
target_link_libraries(mpi_time ${MPI_LIBRARIES})