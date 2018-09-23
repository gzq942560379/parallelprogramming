add_executable(row_matrix_vector_multipy row_matrix_vector_multipy.c)


target_link_libraries(row_matrix_vector_multipy random_uitls)
target_link_libraries(row_matrix_vector_multipy console_utils)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

target_link_libraries(row_matrix_vector_multipy ${MPI_LIBRARIES})
