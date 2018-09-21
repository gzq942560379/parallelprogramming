

add_executable(vector_add vector_add.c)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})
target_link_libraries(vector_add ${MPI_LIBRARIES})

target_link_libraries(vector_add check_error)