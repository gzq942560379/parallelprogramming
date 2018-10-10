include_directories(${PROJECT_SOURCE_DIR}/src/header/pthread)

link_libraries(-lpthread)

add_subdirectory(estimate_pi)
add_subdirectory(thread_create_test)
add_subdirectory(task_queue)
add_subdirectory(read_write_lock)
add_subdirectory(barrier)

