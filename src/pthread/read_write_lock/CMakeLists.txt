
add_executable(rwlock_test rwlock.c)

add_library(rwlock rwlock.c)

target_link_libraries(rwlock_test utils)
