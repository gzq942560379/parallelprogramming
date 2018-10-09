
add_executable(thread_create_test thread_create_test.c thread_create_test.h)

find_package(Threads REQUIRED)
target_link_libraries(thread_create_test Threads::Threads)

target_link_libraries(thread_create_test time_utils)

enable_testing()
add_test(test1 thread_create_test 1000)
add_test(test2 thread_create_test 10000)
add_test(test3 thread_create_test 100000)
add_test(test4 thread_create_test 1000000)
add_test(test5 thread_create_test 10000000)