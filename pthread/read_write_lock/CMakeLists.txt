
add_executable(rwlock_test rwlock.c rwlock.h)

add_library(rwlock rwlock.c rwlock.h)

find_package(Threads REQUIRED)
target_link_libraries(rwlock_test Threads::Threads)
target_link_libraries(rwlock_test random_uitls)