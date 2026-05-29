#ifndef LIST_H
#define LIST_H

#pragma once
#include <stdlib.h>
#include <string.h>

struct List;
typedef struct list list_t;

/*
* callback functions
*/
typedef int (*list_eq)(const void *a, const void *b);
typedef void *(*list_copy_cb)(void *);
typedef void (*list_free_cb)(void *);

typedef struct {
    list_eq leq;
    list_copy_cb lcopy;
    list_free_cb lfree;
} list_cbs_t;

/**
* flags
 */
typedef enum {
    LIST_NONE = 0,
    LIST_SORT = 1 << 0
} list_flags_t;

extern list_t *init_list(const list_cbs_t *cbs, list_flags_t flags);
extern void list_destroy(list_t *l);
#endif