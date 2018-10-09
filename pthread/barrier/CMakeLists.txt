add_executable(barrier_test barrier.c barrier.h)

add_library(barrier barrier.c barrier.h)

find_package(Threads REQUIRED)
target_link_libraries(barrier ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(barrier_test ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(barrier_test random_uitls)