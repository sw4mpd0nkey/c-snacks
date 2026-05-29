#include <stdbool.h>
#include <stddef.h>

#include "list.h"


//https://nachtimwald.com/2020/04/09/generic-list-in-c/

static const size_t list_block_size = 32;
struct list {
    void **elements;
    size_t alloced;
    size_t len;
    list_cbs_t cbs;
    list_flags_t flags;
    bool inbulk;
};

static int list_passthrough_eq(const void *a, const void *b) {
    void *sa = *(void **)a;
    void *sb = *(void **)b;

    return sa - sb;
}

list_t *init_list(const list_cbs_t *cbs, list_flags_t flags) {
    list_t *list;

    list = malloc(sizeof(*list));
    list->elements = malloc(sizeof(*list->elements)*list_block_size);
    list->alloced = list_block_size;
    list->len = 0;

    memset(&(list->cbs), 0, sizeof(list->cbs));
    if(cbs != NULL) {
        list->cbs.leq = cbs->leq;
        list->cbs.lcopy = cbs->lcopy;
        list->cbs.lfree = cbs->lfree;
    }
    if(list->cbs.leq == NULL) {
        list->cbs.leq = list_passthrough_eq;
    }

    list->flags = flags;

    return list;
}