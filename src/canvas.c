#define OLIVEC_IMPLEMENTATION
#include "../include/argon.h"
#include <stdint.h>
#include <stdlib.h>

typedef struct {
  Olivec_Canvas canvas;
} arCanvasData;

void _arCanvas_draw(arView* self, Olivec_Canvas canvas) {
  arCanvasData* data = (arCanvasData*) self->data;
  olivec_sprite_copy(canvas, 0, 0,
    data->canvas.width < canvas.width ? data->canvas.width : canvas.width,
    data->canvas.height < canvas.height ? data->canvas.height : canvas.height,
    data->canvas
  );
}

void _arCanvas_owned_destroy(arView* self) {
  free(((arCanvasData*)self->data)->canvas.pixels);
}

arView* arCanvas_create(size_t width, size_t height) {
  arView* view = arView_create();
  view->destroy = _arCanvas_owned_destroy;
  arCanvasData* data = malloc(sizeof(arCanvasData));
  data->canvas = olivec_canvas(
    malloc(width * height * sizeof(uint32_t)),
    width, height, width);
  view->data = (void*) data;
  view->draw = _arCanvas_draw;

  return view;
}

arView* arCanvas_createWithData(uint32_t* data, size_t width, size_t height) {
  arView* view = arView_create();
  arCanvasData* cdata = malloc(sizeof(arCanvasData));
  cdata->canvas = olivec_canvas(data, width, height, width);
  view->data = (void*) cdata;
  view->draw = _arCanvas_draw;

  return view;
}

arView* arCanvas_createWithCanvas(Olivec_Canvas canvas) {
  arView* view = arView_create();
  arCanvasData* cdata = malloc(sizeof(arCanvasData));
  cdata->canvas = canvas;
  view->data = (void*) cdata;
  view->draw = _arCanvas_draw;

  return view;
}

Olivec_Canvas arCanvas_getCanvas(arView* self) {
  return ((arCanvasData*)self->data)->canvas;
}

uint32_t* arCanvas_getData(arView* self) {
  return ((arCanvasData*)self->data)->canvas.pixels;
}

