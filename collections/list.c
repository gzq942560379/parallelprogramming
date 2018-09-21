//
// Created by gzq on 18-7-12.
//

#include <stdlib.h>
#include <stdio.h>
#include "list.h"


int test() {
    struct List list;
    list_init(&list);
    int add_last[] = {1, 2, 3};
    for (int i = 0; i < 3; i++) {
        list_append(&list, &add_last[i]);
    }
    int add_first[] = {4, 5, 6};
    for (int i = 0; i < 3; i++) {
        list_push(&list, &add_first[i]);
    }
    printf("%d \n", *(int *) list_top(&list));
    printf("%d %d \n", *(int *) list_pop(&list), list_size(&list));
    printf("%d \n", list_size(&list));
    list_print(&list);
    list_destory(&list);
}


int list_size(struct List *list) {
    return list->size;
}

void list_init(struct List *list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void list_push(struct List *list, void *val) {
    struct Node *node = malloc(sizeof(struct Node));
    node->val = val;
    list->size++;
    if (list->head == NULL) {
        list->head = node;
        list->head->next = NULL;
        list->tail = list->head;
        return;
    }
    node->next = list->head;
    list->head = node;
}

void *list_top(struct List *list) {
    if (list->head == NULL) {
        return NULL;
    }
    return list->head->val;
}

void *list_pop(struct List *list) {
    if (list->head == NULL) {
        return NULL;
    }
    void *rtn = list->head->val;
    list->head = list->head->next;
    list->size--;
    return rtn;
}

void list_append(struct List *list, void *val) {
    struct Node *node = malloc(sizeof(struct Node));
    node->val = val;
    list->size++;
    if (list->head == NULL) {
        list->head = node;
        list->head->next = NULL;
        list->tail = list->head;
        return;
    }
    list->tail->next = node;
    list->tail = node;
}

void list_destory(struct List *list) {
    struct Node *next;
    while (list->head != NULL) {
        next = list->head->next;
        free(list->head);
        list->head = next;
    }
}

void list_print(struct List *list) {
    struct Node *cur = list->head;
    while (cur != NULL) {
        printf("%d ", *(int *) cur->val);
        cur = cur->next;
    }
}
