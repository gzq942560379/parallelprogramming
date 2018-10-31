//
// Created by gzq on 18-7-12.
//



#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include "linkedblockqueue.h"

LinkedBlockQueue queue;

/*
void *producer_func() {
    int count = 0;
    while (count++ < 5) {
        long rnd = (long) rand(); //因为在64位 一个void×  八个字节。。。
        printf("produce a product %ld \n", rnd);
        list_block_queue_put(&queue, (void *) rnd);
    }
    return NULL;
}

void *consumer_func() {
    while (true) {
        long get;
        get = list_block_queue_take(&queue);
        printf("coutom a product %ld \n", get);
    }
}

int main() {
    pthread_t consumer, producer;
    pthread_create(&producer, NULL, producer_func, NULL);
    pthread_create(&consumer, NULL, consumer_func, NULL);
    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);
}
*/
size_t list_block_queue_size(LinkedBlockQueue *queue) {
    pthread_mutex_lock(&queue->mutex);
    int rtn = list_size(&queue->list);
    pthread_mutex_unlock(&queue->mutex);
    return rtn;
}


void *list_block_queue_take(LinkedBlockQueue *queue) {
    sem_wait(&queue->full);
    pthread_mutex_lock(&queue->mutex);
    void *val = list_pop(&queue->list);
    pthread_mutex_unlock(&queue->mutex);
    return val;
}

int list_block_queue_put(LinkedBlockQueue *queue, void *val) {
    pthread_mutex_lock(&queue->mutex);
    list_append(&queue->list, val);
    pthread_mutex_unlock(&queue->mutex);
    sem_post(&queue->full);
    return 0;
}

int list_block_queue_init(LinkedBlockQueue *queue) {
    pthread_mutex_init(&(queue->mutex), NULL);
    sem_init(&(queue->full), 0, 0);
    list_init(&(queue->list));
    return 0;
}

int list_block_queue_destory(LinkedBlockQueue *queue) {
    list_destroy(&queue->list);
    pthread_mutex_destroy(&queue->mutex);
    sem_destroy(&queue->full);
    return 0;
}


