#ifndef _AR_ARRAY_H
#define _AR_ARRAY_H

#define AR_DEF_H_ARRAY(ArrName, type) \
  typedef struct _##ArrName { \
    type * values; \
    int count; \
    int cap; \
  } ArrName; \
  ArrName * ArrName##_create(); \
  void ArrName##_destroy(ArrName * self); \
  void ArrName##_assureMinCap(ArrName * self, int cap); \

#define AR_DEF_ARRAY(ArrName, type) \
  ArrName * ArrName##_create() { \
    ArrName * array = malloc(sizeof(ArrName)); \
    array->cap = 0; \
    array->count = 0; \
    array->values = NULL; \
    return array; \
  } \
  void ArrName##_destroy(ArrName * self) { \
    if (self->values != NULL) free(self->values); \
    free(self); \
  } \
  void ArrName##_assureMinCap(ArrName * self, int cap) { \
    if (self->cap <= self->count) \
      if (self->cap < cap) { \
        if (self->cap == 0) { \
          self->cap = cap; \
          self->values = malloc(cap * sizeof(type)); \
        } else { \
          self->cap = cap; \
          self->values = realloc(self->values, cap * sizeof(type)); \
        } \
      } \
  } 

#endif

