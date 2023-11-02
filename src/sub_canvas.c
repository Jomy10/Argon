#include "../include/argon.h"

// Has already been written to the canvas
static void _arSubCanvas_draw(arView* self, Olivec_Canvas canvas) {}

arView* arSubCanvas_create(size_t width, size_t height) {
  arView* view = arView_create();
  view->draw = _arSubCanvas_draw;
  return view;
}

Olivec_Canvas arSubCanvas_getCanvas(arView* self) {
  return self->previous_canvas;
}

