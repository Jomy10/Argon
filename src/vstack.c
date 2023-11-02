
#define OLIVEC_IMPLEMENTATION
#include "../include/argon.h"
#include "int_array.h"

#include <stdlib.h>

static void _arVStack_draw(arView* self, Olivec_Canvas canvas) {
  IntArray* widths = (IntArray*) self->data;
  int y = 0;
  int height;
  arView* child;
  Olivec_Canvas subcanvas;
  for (int i = 0; i < self->children->size; i++) {
    child = self->children->values[i];
    height = widths->values[i < widths->count ? i : widths->count];
    subcanvas = olivec_subcanvas(canvas, 0, y, canvas.width, height);
    arView_draw(child, subcanvas, (arPosition){0, y});
    y += height;
  }
}

arView* arVStack_create() {
  arView* view = arView_create();
  view->draw = _arVStack_draw;
  view->data = malloc(sizeof(IntArray));
  IntArray* data = IntArray_create();
  return view;
}

void arVStack_setHeight(arView* self, int at_index, int height) {
  self->should_rerender = true;
  IntArray* data = ((IntArray*)self->data);
  IntArray_assureMinCap(data, at_index + 1);

  data->values[at_index] = height;
  if (data->count <= at_index) {
    for (int i = data->count; i < at_index; i++) {
      data->values[i] = 0;
    }
    data->count = at_index + 1;
  }
}

void arVStack_assureMinheightCap(arView* self, int cap) {
  IntArray_assureMinCap((IntArray*)self->data, cap);
}

