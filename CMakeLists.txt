cmake_minimum_required(VERSION 3.10)
project(parallelprogramming)

set(CMAKE_CXX_STANDARD 11)

add_executable(parallelprogramming main.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)

add_subdirectory(collections)
add_subdirectory(data_partition)
add_subdirectory(util)

add_subdirectory(pthread)
add_subdirectory(openmp)
add_subdirectory(mpi)



enable_testing()



