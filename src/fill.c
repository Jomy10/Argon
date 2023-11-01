#define OLIVEC_IMPLEMENTATION
#include "../include/argon.h"

#include <stdlib.h>

typedef struct {
  arColor color;
} arFillData;

#include <stdio.h>

static void _arFill_draw(arView* self, Olivec_Canvas canvas) {
  printf("filling 0x%08x in %lu->%lu @ %p\n", ((arFillData*)self->data)->color, canvas.width, canvas.height, canvas.pixels);
  olivec_fill(canvas, ((arFillData*)self->data)->color);
}

arView* arFill_create(arColor color) {
  arView* view = arView_create();
  view->draw = _arFill_draw;
  view->data = malloc(sizeof(arFillData));
  arFillData* data = view->data;
  data->color = color;
  return view;
}

void arFill_setColor(arView* self, arColor color) {
  self->should_rerender = true;
  ((arFillData*)self->data)->color = color;
}

