cmake_minimum_required(VERSION 3.10)
project(parallelprogramming)

set(CMAKE_CXX_STANDARD 11)

SET(CMAKE_C_COMPILER mpicc)
SET(CMAKE_CXX_COMPILER g++)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

add_executable(parallelprogramming main.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)

add_subdirectory(collections)
add_subdirectory(data_partition)
add_subdirectory(util)

add_subdirectory(pthread)
add_subdirectory(openmp)
add_subdirectory(mpi)

enable_testing()



