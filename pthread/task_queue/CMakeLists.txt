

add_executable(task_queue task_queue.c task_queue.h)


add_library(linkedblockqueue linkedblockqueue.c linkedblockqueue.h)

target_link_libraries(linkedblockqueue list)

find_package(Threads REQUIRED)
target_link_libraries(task_queue Threads::Threads)

target_link_libraries(task_queue linkedblockqueue)
target_link_libraries(task_queue random_uitls)


