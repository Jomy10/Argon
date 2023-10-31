#include "int_array.h"

#include <stdlib.h>

IntArray* IntArray_create() {
  IntArray* array = malloc(sizeof(IntArray));
  array->cap = 0;
  array->count = 0;
  array->values = NULL;

  return array;
}

void IntArray_free(IntArray* self) {
  if (self->values != NULL) free(self->values);
  free(self);
}

void IntArray_assureMinCap(IntArray* self, int cap) {
  if (self->cap <= self->count)
    if (self->cap < cap) {
      if (self->cap == 0) {
        self->cap = cap;
        self->values = malloc(cap * sizeof(int));
      } else {
        self->cap = cap;
        self->values = realloc(self->values, cap * sizeof(int));
      }
    }
}

