#ifndef _AR_INT_ARRAY_H
#define _AR_INT_ARRAY_H

typedef struct {
  int* values;
  int count;
  int cap;
} IntArray;

IntArray* IntArray_create();

void IntArray_free(IntArray* self);

void IntArray_assureMinCap(IntArray* self, int cap);

#endif

