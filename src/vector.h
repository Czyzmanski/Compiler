#ifndef VECTOR_H
#define VECTOR_H

#include <assert.h>

#include "command.h"

#define GROWTH_FACTOR 2
#define INITIAL_CAPACITY 32

struct vector {
    command_t **arr;
    int size;
    int capacity;
};

typedef struct vector vector_t;

static inline void init(vector_t *v) {
    v->size = 0;
    v->capacity = INITIAL_CAPACITY;
    v->arr = malloc(v->capacity * sizeof(command_t *));
    assert(v->arr != NULL);
}

static inline void reallocate(vector_t *v) {
    v->capacity = GROWTH_FACTOR * v->capacity;
    v->arr = realloc(v->arr, v->capacity * sizeof(command_t *));
    assert(v->arr != NULL);
}

static inline void add_element(vector_t *v, command_t *elem) {
    if (v->size == v->capacity) {
        reallocate(v);
    }
    v->arr[v->size] = elem;
    v->size++;
}

static inline void dispose(vector_t *v) {
    for (int i = 0; i < v->size; i++) {
        free(v->arr[i]);
    }
    free(v->arr);
}

#endif // VECTOR_H
