//
// Created by gzq on 18-7-13.
//

#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include "task_queue.h"
#include "../../util/random_utils/random_uitls.h"

void *func(void *rank) {
    int rnd = randint(0, 5);
    printf("work rank %ld is processing ... about need %d seconds\n", (long) rank, rnd);
    sleep(rnd);
    return rank;
}

int main() {
    TaskQueue queue;
    task_queue_init(&queue, 3);
    int task_count = 20;
    Future *future = malloc(sizeof(Future) * task_count);
    for (long i = 0; i < task_count; i++) {
        task_queue_submit(&queue, func, (void *) i, &future[i]);
    }
    task_queue_shudown(&queue, true);
    for (int i = 0; i < task_count; i++) {
        printf("work %d get result %ld \n", i, (long) future[i].result);
    }
    free(future);
    task_queue_destory(&queue);
}


void *worker(void *attr) {
    TaskQueue *queue = ((worker_attr *) attr)->queue;
    int rank = ((worker_attr *) attr)->rank;
    free(attr);
    while (!queue->stopped || list_block_queue_size(&queue->queue) != 0) {
        printf("core %d receiving a task\n", rank);
        Task *task = list_block_queue_take(&queue->queue);
        task->future->result = task->task(task->args);
        task->future->done = true;
        printf("core %d complete task %ld\n", rank, task->id);
        free(task);
    }
}

int task_queue_init(TaskQueue *taskQueue, size_t core_size) {
    list_block_queue_init(&taskQueue->queue);
    taskQueue->stopped = false;
    taskQueue->counter = 0;
    taskQueue->core_size = core_size;
    taskQueue->cores = malloc(sizeof(pthread_t) * core_size);
    for (int i = 0; i < core_size; i++) {
        worker_attr *attr = malloc(sizeof(worker_attr));
        attr->queue = taskQueue;
        attr->rank = i;
        pthread_create(&taskQueue->cores[i], NULL, worker, attr);
    }
    return 0;
}

int task_queue_destory(TaskQueue *taskQueue) {
    list_block_queue_destory(&taskQueue->queue);
    free(taskQueue->cores);
    return 0;
}

int task_queue_submit(TaskQueue *taskQueue, void *(*work)(void *), void *args, Future *future) {
    if (taskQueue->stopped) {
        perror("task queue has been stopped !!!");
        return 1;
    }
    Task *task = malloc(sizeof(Task));
    task->id = taskQueue->counter++;
    task->task = work;
    task->args = args;
    task->future = future;
    task->future->done = false;
    list_block_queue_put(&taskQueue->queue, task);
    return 0;
}

int task_queue_shudown(TaskQueue *taskQueue, bool wait) {
    taskQueue->stopped = true;
    if (wait) {
        for (int i = 0; i < taskQueue->core_size; i++) {
            pthread_join(taskQueue->cores[i], NULL);
        }
    }
    return 0;
}



