#ifndef _AR_INT_ARRAY_H
#define _AR_INT_ARRAY_H

#include "array.h"

AR_DEF_H_ARRAY(IntArray, int);

/*
typedef struct {
  int* values;
  int count;
  int cap;
} IntArray;

IntArray* IntArray_create();

void IntArray_free(IntArray* self);

void IntArray_assureMinCap(IntArray* self, int cap);
*/

#endif

