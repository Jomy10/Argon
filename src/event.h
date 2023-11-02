#ifndef _AR_EVENT_H
#define _AR_EVENT_H

#include "../include/argon.h"

#include "array.h"

typedef struct {
  enum {
    AR_DISPEV_CLICK,
  } type;
  union {
    struct {
      arPosition start_position;
      arPosition end_position;
    } click;
  } data;
} arDispatchedEvent;

AR_DEF_H_ARRAY(arDispatchedEventArray, arDispatchedEvent);

void arEvent_handleAt(ArgonUI* state, arView* view, arPosition pos, arSize size);

/// Call when view is clicked
void arView_onClick(arView* self);

void argon_clearEvents(ArgonUI* self);

// typedef struct _arEventArray {
//   arEvent* values;
//   int count;
//   int cap;
// } arEventArray;
//
// arEventArray* arEventArray_create();
//
// void arEventArray_destroy(arEventArray*);
//
// void arEventArray_assureMinCap(arEventArray*, int cap);

#endif

