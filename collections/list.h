//
// Created by gzq on 18-7-12.
//

#ifndef PTHREAD_DEMO_LIST_H
#define PTHREAD_DEMO_LIST_H


#include <stddef.h>

struct Node {
    void *val;
    struct Node *next;
} node;

struct List {
    struct Node *head;
    struct Node *tail;
    size_t size;
} list;

int list_size(struct List *list);

void list_init(struct List *list);

void list_push(struct List *list, void *val);

void *list_top(struct List *list);

void *list_pop(struct List *list);

void list_append(struct List *list, void *val);

void list_destory(struct List *list);

void list_print(struct List *list);


#endif //PTHREAD_DEMO_LIST_H
