

add_executable(task_queue task_queue.c)

add_library(linkedblockqueue linkedblockqueue.c)

target_link_libraries(linkedblockqueue list)

target_link_libraries(task_queue linkedblockqueue)
target_link_libraries(task_queue utils)


