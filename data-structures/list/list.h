#ifndef LIST_H
#define LIST_H

#pragma once
#include <stdlib.h>

typedef enum {
    SUCCESS = 0,
    FAILURE = -1,
} return_t;

typedef struct node {
    
    void* data;
    struct node* next;
    struct node* prev;

} list_node_t;

typedef struct list {
    
    int size;
    struct node* head;
    struct node* tail;

} list_t;


list* create_list();
int destroy_list(list*);
int clear_list(list*);
int size_list(list*);

void* head_list(list*);
void* tail_list(list*);
int set_list(list*, int index, void* data);
void* get_list(list*, int index);

#endif