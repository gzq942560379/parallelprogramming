//
// Created by gzq on 18-7-12.
//

#ifndef PTHREAD_DEMO_LINKEDBLOCKQUEUE_H
#define PTHREAD_DEMO_LINKEDBLOCKQUEUE_H

#include <semaphore.h>
#include "../..//collections/list/list.h"
#include "linkedblockqueue.h"

typedef struct LinkedBlockQueue {
  struct List list;
  sem_t full;  //队列中有多少资源可用
  pthread_mutex_t mutex;
} LinkedBlockQueue;

size_t list_block_queue_size(LinkedBlockQueue *queue);

int list_block_queue_init(LinkedBlockQueue *queue);

void *list_block_queue_take(LinkedBlockQueue *queue);

int list_block_queue_put(LinkedBlockQueue *queue, void *val);

int list_block_queue_destory(LinkedBlockQueue *queue);

#endif  // PTHREAD_DEMO_LINKEDBLOCKQUEUE_H
