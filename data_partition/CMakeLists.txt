

add_executable(block_data_partition data_partition.c data_partition.h)

find_package(Threads REQUIRED)
target_link_libraries(block_data_partition Threads::Threads)