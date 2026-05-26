#include "vector.h"

#include <stdio.h>
#include <assert.h>

void init(Vector *vec, int data) {
    vec -> contents = (unsigned int*)malloc(sizeof(int));
    vec -> contents[0] = data;
    vec -> current = 0;
    vec -> size = 1;
}

void destroy(Vector *vec) {
    free(vec -> contents);
    
}

void clear(Vector* vec) {
    destroy(vec);
    init(vec,0);
}

void add(Vector* vec, int val) {
    vec -> contents = realloc(vec->contents, (sizeof(unsigned int) * (vec -> size +1)));
    vec -> contents[vec -> size] = val;
    vec -> size++;
}

int get(Vector* vec, int index) {
    if(index < vec -> size) {
        return vec -> contents[index];
    }

    return -1;
}

void set(Vector* vec, int index, int val) {
    if(index < vec -> size) {
        vec -> contents[index] = val;
    }
}

int next(Vector* vec) {
    if(vec -> current == vec -> size) {
        vec -> current = 0;
    }

    int toReturn = vec -> contents[vec -> current];
    vec -> current++;
    return toReturn;
}

int size(Vector* vec) {
    return vec -> size;
}

static void test() {
    Vector test;
    init(&test, 6);
    printf("checking get functionality");
    assert(get(&test, 0) == 6);
}