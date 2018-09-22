//
// Created by gzq on 18-7-15.
//

#ifndef PTHREAD_DEMO_DATA_PARTITION_H
#define PTHREAD_DEMO_DATA_PARTITION_H

#define CROSS_BLOCK_LOW(id, p, n) ((id) * (n) / (p))
#define CROSS_BLOCK_HIGH(id, p, n) (CROSS_BLOCK_LOW((id) + 1, p, n) - 1)
#define CROSS_BLOCK_SIZE(id, p, n) (CROSS_BLOCK_LOW((id)+1,p,n)-(CROSS_BLOCK_LOW(id,p,n)
#define CROSS_BLOCK_OWNER(index, p, n) (((p) * ((index) + 1) - 1) / (n))

/*
 *   分块结果 [start,end]
 */
typedef struct BlockPartition {
  long start;
  long end;
} BlockPartition;

int block_data_partition(long block_num, long thread_count,
                         BlockPartition **retval);

int cross_data_partition(long num, long thread_count, BlockPartition **retval);

void print_partition(BlockPartition *partitions, int thread_count);
#endif  // PTHREAD_DEMO_DATA_PARTITION_H
