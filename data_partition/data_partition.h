//
// Created by gzq on 18-7-15.
//

#ifndef PTHREAD_DEMO_DATA_PARTITION_H
#define PTHREAD_DEMO_DATA_PARTITION_H

/*
 *   分块结果 [start,end)
 */
typedef struct BlockPartition {
    long start;
    long end;
} BlockPartition;

int block_data_partition(long block_num, long thread_count, BlockPartition **retval);



#endif //PTHREAD_DEMO_DATA_PARTITION_H
