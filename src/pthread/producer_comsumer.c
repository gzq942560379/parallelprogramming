//
// Created by gzq on 18-10-31.
//
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <zconf.h>

#include "list.h"

#define PRODUCER_NUM 1
#define CONSUMER_NUM 2

#define BUFFER_SIZE 16

typedef struct {
    pthread_mutex_t *buffer_mutex_p;
    pthread_cond_t *cond_p;
    list_t *list_p;
    int rank;
} param_t;

void producer(param_t *param) {
    pthread_mutex_t *buffer_mutex_p = param->buffer_mutex_p;
    pthread_cond_t *cond_p = param->cond_p;
    list_t *list_p = param->list_p;
    int rank = param->rank;
    long count = 0;
    long product;

    while (true) {
        // produce
        product = count++;
        pthread_mutex_lock(buffer_mutex_p);
        while (list_size(list_p) == BUFFER_SIZE) {
            while (pthread_cond_wait(cond_p, buffer_mutex_p) != 0);
        }
        list_append(list_p, (void *) product);
        pthread_cond_broadcast(cond_p);
        pthread_mutex_unlock(buffer_mutex_p);
    }
}

void consumer(param_t *param) {
    pthread_mutex_t *buffer_mutex_p = param->buffer_mutex_p;
    pthread_cond_t *cond_p = param->cond_p;
    list_t *list_p = param->list_p;
    int rank = param->rank;
    long count = 0;
    long product;
    while (true) {
        pthread_mutex_lock(buffer_mutex_p);
        while (list_size(list_p) == 0) {
            while (pthread_cond_wait(cond_p, buffer_mutex_p) != 0);
        }
        product = (long) list_pop(list_p);
        printf("consumer %d get product %ld\n", rank, product);

        pthread_cond_broadcast(cond_p);
        pthread_mutex_unlock(buffer_mutex_p);
    }
}

int main(int argc, char **argv) {
    list_t list;
    pthread_mutex_t buffer_mutex;
    pthread_cond_t cond;

    list_init(&list);
    pthread_mutex_init(&buffer_mutex, NULL);
    pthread_cond_init(&cond, NULL);

    int total_thread_count = PRODUCER_NUM + CONSUMER_NUM;

    pthread_t *threads = malloc(total_thread_count * sizeof(pthread_t));
    pthread_t *producer_threads = threads, *consumer_threads = threads + PRODUCER_NUM;
    param_t *params = malloc(total_thread_count * sizeof(param_t));
    param_t *producer_params = params, *consumer_params = params + PRODUCER_NUM;
    for (int i = 0; i < PRODUCER_NUM; ++i) {
        producer_params[i].rank = i;
        producer_params[i].buffer_mutex_p = &buffer_mutex;
        producer_params[i].cond_p = &cond;
        producer_params[i].list_p = &list;
        pthread_create(&producer_threads[i], NULL, producer, &producer_params[i]);
    }
    for (int i = 0; i < CONSUMER_NUM; ++i) {
        consumer_params[i].rank = i;
        consumer_params[i].buffer_mutex_p = &buffer_mutex;
        consumer_params[i].cond_p = &cond;
        consumer_params[i].list_p = &list;
        pthread_create(&consumer_threads[i], NULL, consumer, &consumer_params[i]);
    }
    for (int i = 0; i < total_thread_count; ++i) {
        pthread_join(threads[i], NULL);
    }


    pthread_mutex_destroy(&buffer_mutex);
    pthread_cond_destroy(&cond);
    list_destroy(&list);

    free(threads);
    return 0;
}