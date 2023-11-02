#include "../include/argon.h"

#include <stdlib.h>

#define OLIVEC_IMPLEMENTATION
#include <olive.c>

typedef struct {
  size_t w, h;
  arMargin margin;
} arContainerData;

static void _arContainer_draw(arView* self, Olivec_Canvas maincanvas) {
  arContainerData* data = ((arContainerData*)self->data);
  Olivec_Canvas canvas = olivec_subcanvas(
    maincanvas,
    data->margin.l,
    data->margin.u,
    data->w - data->margin.r - data->margin.l,
    data->h - data->margin.d - data->margin.u);
  arPosition dispatchPos = self->current_pos;
  // TODO: specify position of child everywhere,
  // then have a argon_handleEvents() function to handle events according to 
  // current_pos and previous_canvas.width/height
  // ---> check all dispatched events at once to avoid having to follow pointers too much
  dispatchPos.x += data->margin.l;
  dispatchPos.y += data->margin.u;
  for (int i = 0; i < self->children->size; i++) {
    arView_draw(self->children->values[i], canvas, dispatchPos);
  }
}

arView* arContainer_create(size_t w, size_t h) {
  arView* view = arView_create();
  view->draw = _arContainer_draw;
  view->data = malloc(sizeof(arContainerData));
  arContainerData* data = view->data;
  data->w = w;
  data->h = h;
  data->margin = (arMargin){0};
  return view;
}

void arContainer_setWidth(arView* self, size_t w) {
  self->should_rerender = true;
  ((arContainerData*)self->data)->w = w;
}

void arContainer_setHeight(arView* self, size_t h) {
  self->should_rerender = true;
  ((arContainerData*)self->data)->h = h;
}

void arContainer_setMargin(arView* self, arMargin margin) {
  self->should_rerender = true;
  ((arContainerData*)self->data)->margin = margin;
}

arMargin arContainer_getMargin(arView* self) {
  return ((arContainerData*)self->data)->margin;
}

