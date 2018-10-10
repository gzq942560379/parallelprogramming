add_executable(barrier_test barrier.c)

add_library(barrier barrier.c)

target_link_libraries(barrier_test utils)