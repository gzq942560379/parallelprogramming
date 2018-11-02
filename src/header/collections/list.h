//
// Created by gzq on 18-7-12.
//

#ifndef PTHREAD_DEMO_LIST_H
#define PTHREAD_DEMO_LIST_H

#include <stddef.h>

typedef struct {
    void *val;
    struct Node *next;
} node_t;

typedef struct {
    node_t *head;
    node_t *tail;
    size_t size;
} list_t;

int list_size(list_t *list);

void list_init(list_t *list);

void list_destroy(list_t *list);

void list_push(list_t *list, void *val);

void *list_top(list_t *list);

void *list_pop(list_t *list);

void list_append(list_t *list, void *val);

void list_print(list_t *list);



#endif  // PTHREAD_DEMO_LIST_H
