cmake_minimum_required(VERSION 3.12)
project(parallelprogramming)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(collections)
add_subdirectory(mpi)
add_subdirectory(openmp)
add_subdirectory(pthread)
add_subdirectory(template)
add_subdirectory(util)

add_executable(parallelprogramming main.cpp)
