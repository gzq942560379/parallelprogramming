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

#include "list.h"
#include "time_utils.h"

#define CONSUMER_NUM 2

#define BUFFER_SIZE 16

typedef struct {
    pthread_mutex_t *buffer_mutex_p;
    pthread_cond_t *cond_p;
    list_t *list_p;
    long total;
    bool *completed
} producer_param_t;

typedef struct {
    pthread_mutex_t *buffer_mutex_p;
    pthread_cond_t *cond_p;
    list_t *list_p;
    int rank;
    bool *completed
} consumer_param_t;

void producer(producer_param_t *param) {
    pthread_mutex_t *buffer_mutex_p = param->buffer_mutex_p;
    pthread_cond_t *cond_p = param->cond_p;
    list_t *list_p = param->list_p;
    bool *completed = param->completed;
    long count = 0;
    long total = param->total;
    long product;

    while (true) {
        // produce
        product = count++;

        pthread_mutex_lock(buffer_mutex_p);
        if (product == total) {
            *completed = true;
            printf("product complete!!!\n");
            pthread_cond_broadcast(cond_p);
            pthread_mutex_unlock(buffer_mutex_p);
            return;
        }
        while (list_size(list_p) == BUFFER_SIZE) {
            while (pthread_cond_wait(cond_p, buffer_mutex_p) != 0);
        }
        list_append(list_p, (void *) product);
//        printf("produce\n");
        pthread_cond_broadcast(cond_p);
        pthread_mutex_unlock(buffer_mutex_p);
    }
}

void consumer(consumer_param_t *param) {
    pthread_mutex_t *buffer_mutex_p = param->buffer_mutex_p;
    pthread_cond_t *cond_p = param->cond_p;
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
                pthread_cond_broadcast(cond_p);
                pthread_mutex_unlock(buffer_mutex_p);
                return;
            }
            while (pthread_cond_wait(cond_p, buffer_mutex_p) != 0);
        }
        product = (long) list_pop(list_p);
//        printf("consumer %d get product %ld\n", rank, product);
        pthread_cond_broadcast(cond_p);
        pthread_mutex_unlock(buffer_mutex_p);
    }
}

int main(int argc, char **argv) {
    long total = 10000000;
    list_t list;
    pthread_mutex_t buffer_mutex;
    pthread_cond_t cond;

    list_init(&list);
    pthread_mutex_init(&buffer_mutex, NULL);
    pthread_cond_init(&cond, NULL);

    bool completed = false;

    pthread_t *consumer_threads = malloc(CONSUMER_NUM * sizeof(pthread_t));
    consumer_param_t *consumer_params = malloc(CONSUMER_NUM * sizeof(consumer_param_t));
    pthread_t producer_thread;

    producer_param_t producer_param;
    producer_param.buffer_mutex_p = &buffer_mutex;
    producer_param.cond_p = &cond;
    producer_param.list_p = &list;
    producer_param.total = total;
    producer_param.completed = &completed;
    pthread_create(&producer_thread, NULL, producer, &producer_param);


    timer timer;
    start(&timer);
    for (int i = 0; i < CONSUMER_NUM; ++i) {
        consumer_params[i].rank = i;
        consumer_params[i].buffer_mutex_p = &buffer_mutex;
        consumer_params[i].cond_p = &cond;
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
    pthread_cond_destroy(&cond);
    list_destroy(&list);
    free(consumer_params);
    return 0;
}