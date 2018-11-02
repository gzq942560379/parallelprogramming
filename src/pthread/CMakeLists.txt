include_directories(${PROJECT_SOURCE_DIR}/src/header/pthread)

link_libraries(-lpthread)

add_executable(thread_create_test thread_create_test.c)
target_link_libraries(thread_create_test utils)

add_executable(task_queue task_queue.c)
add_library(linkedblockqueue linkedblockqueue.c)
target_link_libraries(linkedblockqueue list)
target_link_libraries(task_queue linkedblockqueue)
target_link_libraries(task_queue utils)

add_executable(rwlock_test rwlock.c)
add_library(rwlock rwlock.c)
target_link_libraries(rwlock_test utils)

add_executable(estimate_pi estimate_pi.c)

add_executable(barrier_test barrier.c)
add_library(barrier barrier.c)
target_link_libraries(barrier_test utils)


add_executable(producer_comsumer producer_comsumer_cond.c)
target_link_libraries(producer_comsumer list utils)

