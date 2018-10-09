# add_executable(check_error_main check_error.c check_error.h)
add_library(check_error check_error.c check_error.h)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})
target_link_libraries(check_error ${MPI_LIBRARIES})