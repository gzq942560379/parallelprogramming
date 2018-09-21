//
// Created by gzq on 18-7-13.
//

#ifndef PTHREAD_DEMO_TASK_QUEUE_H
#define PTHREAD_DEMO_TASK_QUEUE_H

#include <sys/param.h>
#include "linkedblockqueue.h"

typedef struct TaskQueue {
    LinkedBlockQueue queue;
    size_t core_size;
    pthread_t *cores;
    long counter;
    bool stopped;
} TaskQueue;

typedef struct worker_attr {
    TaskQueue *queue;
    int rank;
} worker_attr;

typedef struct Future {
    bool done;
    void *result;
} Future;

typedef struct Task {
    long id;

    void *(*task)(void *);

    void *args;
    Future *future;
} Task;


int task_queue_init(TaskQueue *taskQueue, size_t core_size);

int task_queue_destory(TaskQueue *taskQueue);

int task_queue_submit(TaskQueue *taskQueue, void *(*work)(void *), void *args, Future *future);

int task_queue_shudown(TaskQueue *taskQueue, bool wait);


#endif //PTHREAD_DEMO_TASK_QUEUE_H
