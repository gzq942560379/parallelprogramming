//
// Created by gzq on 18-7-15.
//



#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "data_partition.h"


int main(int argc, char **argv) {

    long block_num = 14;
    long thread_count = 4;

    if (argc >= 2) {
        block_num = atol(argv[1]);
        thread_count = atol(argv[2]);
    }

    BlockPartition *retval;
    block_data_partition(block_num, thread_count, &retval);

    printf("allocate %ld data block to %ld threads...\n", block_num, thread_count);
    long allocated_block = 0;
    for (long rank = 0; rank < thread_count; rank++) {
        printf("thread %ld , allocate data from %ld to %ld ,count is %ld \n", rank, retval[rank].start,
               retval[rank].end - 1,
               retval[rank].end - retval[rank].start);
        allocated_block += retval[rank].end - retval[rank].start;
    }
    printf("allocated block count is %ld \n", allocated_block);
}


int block_data_partition(long block_num, long thread_count, BlockPartition **retval) {
    //assert retval has not been initialized and length will be thread_count
    *retval = malloc(sizeof(BlockPartition) * thread_count);
    ldiv_t d = ldiv(block_num, thread_count);
    for (long rank = 0; rank < thread_count; rank++) {
        BlockPartition partition;
        if (rank < d.rem) {
            partition.start = rank * (d.quot + 1);
            partition.end = partition.start + d.quot + 1;
        } else {
            partition.start = rank * d.quot + d.rem;
            partition.end = partition.start + d.quot;
        }
        (*retval)[rank] = partition;
    }
    return 0;
}

