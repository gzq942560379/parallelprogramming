//
// Created by gzq on 18-10-31.
//

/*
 * 单生产者多消费者
 */
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <zconf.h>
#include <bits/semaphore.h>
#include <semaphore.h>

#include "list.h"
#include "time_utils.h"

#define CONSUMER_NUM 2

#define BUFFER_SIZE 16

typedef struct {
    pthread_mutex_t *buffer_mutex_p;
    list_t *list_p;
    sem_t *empty;
    sem_t *full;
    long total;
    bool *completed
} producer_param_t;

typedef struct {
    pthread_mutex_t *buffer_mutex_p;
    list_t *list_p;
    sem_t *empty;
    sem_t *full;
    int rank;
    bool *completed
} consumer_param_t;

void producer(producer_param_t *param) {
    pthread_mutex_t *buffer_mutex_p = param->buffer_mutex_p;
    sem_t *empty_p = param->empty, *full_p = param->full;
    list_t *list_p = param->list_p;
    bool *completed = param->completed;
    long count = 0;
    long total = param->total;
    long product;

    while (true) {
        // produce
        product = count++;

        if (product == total) {
            *completed = true;
            printf("product complete!!!\n");
            return;
        }
        list_append(list_p, (void *) product);

    }
}

void consumer(consumer_param_t *param) {
    pthread_mutex_t *buffer_mutex_p = param->buffer_mutex_p;

    list_t *list_p = param->list_p;
    int rank = param->rank;
    bool *completed = param->completed;
    long count = 0;
    long product;
    while (true) {
        pthread_mutex_lock(buffer_mutex_p);
        while (list_size(list_p) == 0) {
            if (*completed) {
                printf("consumer %d complete!!!\n", rank);
                pthread_mutex_unlock(buffer_mutex_p);
                return;
            }
        }
        product = (long) list_pop(list_p);
//        printf("consumer %d get product %ld\n", rank, product);
        pthread_mutex_unlock(buffer_mutex_p);
    }
}

int main(int argc, char **argv) {
    long total = 10000000;
    list_t list;
    pthread_mutex_t buffer_mutex;
    sem_t empty, full;

    list_init(&list);
    pthread_mutex_init(&buffer_mutex, NULL);
    sem_init(&empty, NULL, BUFFER_SIZE);
    sem_init(&full, NULL, 0);
    bool completed = false;

    pthread_t *consumer_threads = malloc(CONSUMER_NUM * sizeof(pthread_t));
    consumer_param_t *consumer_params = malloc(CONSUMER_NUM * sizeof(consumer_param_t));
    pthread_t producer_thread;

    producer_param_t producer_param;
    producer_param.buffer_mutex_p = &buffer_mutex;
    producer_param.empty = &empty;
    producer_param.full = &full;
    producer_param.list_p = &list;
    producer_param.total = total;
    producer_param.completed = &completed;
    pthread_create(&producer_thread, NULL, producer, &producer_param);


    timer timer;
    start(&timer);
    for (int i = 0; i < CONSUMER_NUM; ++i) {
        consumer_params[i].rank = i;
        consumer_params[i].buffer_mutex_p = &buffer_mutex;
        consumer_params[i].empty = &empty;
        consumer_params[i].full = &full;
        consumer_params[i].list_p = &list;
        consumer_params[i].completed = &completed;
        pthread_create(&consumer_threads[i], NULL, consumer, &consumer_params[i]);
    }

    pthread_join(producer_thread, NULL);
    for (int i = 0; i < CONSUMER_NUM; ++i) {
        pthread_join(consumer_threads[i], NULL);
    }
    end(&timer);
    print_milli_interval(&timer);


    pthread_mutex_destroy(&buffer_mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    list_destroy(&list);
    free(consumer_params);
    return 0;
}