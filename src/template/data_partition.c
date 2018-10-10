//
// Created by gzq on 18-7-15.
//
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

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

int main(int argc, char **argv) {
  long block_num = 14;
  long thread_count = 4;

  if (argc >= 2) {
    block_num = atol(argv[1]);
    thread_count = atol(argv[2]);
  }
  BlockPartition *retval;
  retval = malloc(sizeof(BlockPartition) * thread_count);

  block_data_partition(block_num, thread_count, &retval);

  print_partition(retval, thread_count);

  cross_data_partition(block_num, thread_count, &retval);
  print_partition(retval, thread_count);

  free(retval);
}

int block_data_partition(long block_num, long thread_count,
                         BlockPartition **retval) {
  // assert retval has not been initialized and length will be thread_count
  ldiv_t d = ldiv(block_num, thread_count);
  for (long rank = 0; rank < thread_count; rank++) {
    BlockPartition partition;
    if (rank < d.rem) {
      partition.start = rank * (d.quot + 1);
      partition.end = partition.start + d.quot;
    } else {
      partition.start = rank * d.quot + d.rem;
      partition.end = partition.start + d.quot - 1;
    }
    (*retval)[rank] = partition;
  }
  printf("block_data_partition : allocate %ld data to %ld threads...\n",
         block_num, thread_count);
  return 0;
}

int cross_data_partition(long num, long thread_count, BlockPartition **retval) {
  for (long rank = 0; rank < thread_count; rank++) {
    BlockPartition partition;
    partition.start = (rank * num) / thread_count;
    partition.end = ((rank + 1) * num) / thread_count - 1;
    (*retval)[rank] = partition;
  }
  printf("cross_data_partition : allocate %ld data to %ld threads...\n", num,
         thread_count);
  return 0;
}

void print_partition(BlockPartition *partitions, int thread_count) {
  long allocated_block = 0;
  long count = 0;
  for (long rank = 0; rank < thread_count; rank++) {
    count = partitions[rank].end - partitions[rank].start + 1;
    printf("thread %ld , allocate data from %ld to %ld ,count is %ld \n", rank,
           partitions[rank].start, partitions[rank].end, count);
    allocated_block += count;
  }
  printf("allocated block count is %ld \n", allocated_block);
}
