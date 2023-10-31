#include "vec.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

struct _arvec* arvec_create_m(int cap, int type_size) {
    //void* mem = malloc(sizeof(struct Vec) + type_size * cap);
    struct _arvec* v = malloc(sizeof(struct _arvec));
    if (cap > 0) {
        v->values = malloc(type_size * cap);
    } else {
        v->values = NULL;
    }
    v->size = 0;
    v->cap = cap;
    v->type_size = type_size;
    return v;
}

void arvec_free(struct _arvec* vec) {
    free(vec->values);
    free(vec);
}

void arvec_assureMinCap(struct _arvec* self, int cap) {
    if (cap < self->cap) return;
        self->cap = cap;
    if (self->cap == 0) {
        self->values = (char*) malloc(self->cap * self->type_size);
    } else {
        self->values = (char*) realloc(self->values, self->cap * self->type_size);
    }
}

void arvec_append(Vec(void) vec, void* value) {
    assert(vec->type_size != 0);

    if (vec->size == vec->cap)
        arvec_assureMinCap(vec, vec->cap == 0 ? 2 : vec->cap * 2);
    // if (vec->size == vec->cap) {
    //     if (vec->cap == 0) {
    //         vec->cap = 3;
    //         vec->values = (char*) malloc(vec->cap * vec->type_size);
    //         printf("new cap: %i\n", vec->cap);
    //     } else {
    //         vec->cap *= 2;
    //         vec->values = (char*) realloc(vec->values, vec->cap * vec->type_size);
    //         printf("new cap2: %i\n", vec->cap);
    //     }
    // }
    memcpy(&((char*)vec->values)[(vec->size++) * vec->type_size], value, vec->type_size);
    printf("new at index %i (size: %d)\n", (vec->size - 1) * vec->type_size, vec->type_size);
    fflush(stdout);
}

void arvec_clear(struct _arvec* vec) {
  vec->size = 0;
}
