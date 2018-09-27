cmake_minimum_required(VERSION 3.10)
project(parallelprogramming)

set(CMAKE_CXX_STANDARD 14)

SET(ENV{CC} /usr/bin/mpicc)
SET(ENV{CXX} /usr/bin/g++)
SET(CMAKE_C_COMPILER mpicc)
SET(CMAKE_CXX_COMPILER g++)

add_subdirectory(collections)
add_subdirectory(mpi)
add_subdirectory(openmp)
add_subdirectory(pthread)
add_subdirectory(template)
add_subdirectory(util)

add_executable(parallelprogramming main.cpp)
