cmake_minimum_required(VERSION 3.10)
project(parallelprogramming)



set(CMAKE_CXX_STANDARD 14 CACHE INTERNAL global)

SET(CMAKE_C_COMPILER gcc CACHE INTERNAL global)
SET(CMAKE_CXX_COMPILER g++ CACHE INTERNAL global)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE INTERNAL global)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE INTERNAL global)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin CACHE INTERNAL global)

set(CMAKE_BUILD_TYPE Debug CACHE INTERNAL global)

set(CMAKE_VERBOSE_MAKEFILE on CACHE INTERNAL global)

SET(CMAKE_C_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb" CACHE INTERNAL global)
SET(CMAKE_C_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall" CACHE INTERNAL global)

SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb" CACHE INTERNAL global)
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall" CACHE INTERNAL global)

SET(ENV{CC} /usr/bin/mpicc)
SET(ENV{CXX} /usr/bin/g++)
SET(CMAKE_C_COMPILER mpicc)
SET(CMAKE_CXX_COMPILER g++)

add_subdirectory(collections)
add_subdirectory(mpi)
add_subdirectory(openmp)
add_subdirectory(pthread)
add_subdirectory(avx)
add_subdirectory(template)
add_subdirectory(util)

add_executable(parallelprogramming main.cpp)