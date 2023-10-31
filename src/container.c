#include "../include/argon.h"

#include <stdlib.h>

#define OLIVEC_IMPLEMENTATION
#include <olive.c>

typedef struct {
  size_t w, h;
} arContainerData;

static void _arContainer_draw(arView* self, Olivec_Canvas maincanvas) {
  arContainerData* data = ((arContainerData*)self->data);
  Olivec_Canvas canvas = olivec_subcanvas(maincanvas, 0, 0, data->w, data->h);
  for (int i = 0; i < self->children->size; i++) {
    arView_draw(self->children->values[i], canvas);
  }
}

arView* arContainer_create(size_t w, size_t h) {
  arView* view = arView_create();
  view->draw = _arContainer_draw;
  view->data = malloc(sizeof(arContainerData));
  arContainerData* data = view->data;
  data->w = w;
  data->h = h;
  return view;
}

void arContainer_setWidth(arView* self, size_t w) {
  ((arContainerData*)self->data)->w = w;
}

void arContainer_setHeight(arView* self, size_t h) {
  ((arContainerData*)self->data)->h = h;
}

