#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "list.h"

struct list* create_list() {

    struct list* new_list = malloc(sizeof(list_t));
    if (new_list == NULL) {
        perror("Error initilizing new list");
        return NULL;
    }

    new_list -> head = NULL;
    new_list -> tail = NULL;
    new_list -> size = 0;

    return new_list;
}

int clear_list(struct list* list) {

    if(list == NULL) {
        perror("List to clear is NULL!");
        return FAILURE;
    }

    struct node* current = list->head;
    while(current != NULL) {
        struct node* to_free = current;
        current = current->next;
        free(to_free);
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

int destroy_list(struct list* list) {
    if (list == NULL) {
        perror("List to destory is NULL!");
        return FAILURE;
    }

    clear_list(list);
    free(list);

    return SUCCESS;
}

int size_list(struct list* list) {
    
    if (list == NULL) {
        perror("List is NULL for size function!");
        return FAILURE;
    }
    
    return list->size;
}

void* head_list(struct list* list) {
    
    if (list == NULL) {
        perror("List is NULL for front function!");
        return NULL;
    }

    if (size_list(list) == 0) {
        return NULL;
    }

    return list->head->data;
}

void* tail_list(struct list* list) {
    
    if (list == NULL) {
        perror("List is NULL for front function!");
        return NULL;
    }

    if (size_list(list) == 0) {
        return NULL;
    }

    return list->tail->data;
}

int set_list(struct list* list, int index, void* data) {

     if (list == NULL) {
        perror("List is NULL for front function!");
        return FAILURE;
    }

    if (index < 0 || index > size_list(list)) {
        perror("set_list invalid index!");
        return FAILURE;
    }

    struct node * current = head_list(list);
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    
    current->data = data;

    return SUCCESS;
}

void* get_list(struct list* list, int index) {

     if (list == NULL) {
        perror("List is NULL for front function!");
        return NULL;
    }

     if (index < 0 || index > size_list(list)) {
        perror("set_list invalid index!");
        return FAILURE;
    }

    struct node * current = head_list(list);
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    
    return current->data;
}