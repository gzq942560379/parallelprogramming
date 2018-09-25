#!/usr/bin/env bash

cd cmake-build-debug

make

cd ..

#mpiexec -n 4 cmake-build-debug/mpi/matrix_vector_multiply/block_matrix_vector_multiply
mpiexec -n 4 cmake-build-debug/mpi/combination_search/mpi_backtrack_permutation