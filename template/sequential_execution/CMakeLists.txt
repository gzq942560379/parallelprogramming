add_executable(sequential_execution sequential_execution.c)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})
target_link_libraries(sequential_execution ${MPI_LIBRARIES})