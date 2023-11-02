#define OLIVEC_IMPLEMENTATION
#include "../include/argon.h"
#include "int_array.h"

#include <stdlib.h>

static void _arHStack_draw(arView* self, Olivec_Canvas canvas) {
  IntArray* widths = (IntArray*) self->data;
  int x = 0;
  int width;
  arView* child;
  Olivec_Canvas subcanvas;
  for (int i = 0; i < self->children->size; i++) {
    child = self->children->values[i];
    width = widths->values[i < widths->count ? i : widths->count];
    subcanvas = olivec_subcanvas(canvas, x, 0, width, canvas.height);
    arView_draw(child, subcanvas, (arPosition){x, 0});
    x += width;
  }
}

arView* arHStack_create() {
  arView* view = arView_create();
  view->draw = _arHStack_draw;
  view->data = malloc(sizeof(IntArray));
  IntArray* data = IntArray_create();
  return view;
}

void arHStack_setWidth(arView* self, int at_index, int width) {
  self->should_rerender = true;
  IntArray* data = ((IntArray*)self->data);
  IntArray_assureMinCap(data, at_index + 1);

  data->values[at_index] = width;
  if (data->count <= at_index) {
    for (int i = data->count; i < at_index; i++) {
      data->values[i] = 0;
    }
    data->count = at_index + 1;
  }
}

void arHStack_assureMinWidthCap(arView* self, int cap) {
  IntArray_assureMinCap((IntArray*)self->data, cap);
}

