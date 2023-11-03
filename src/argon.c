#include "event.h"

#include "../include/argon.h"
#include "children_list.h"

#define OLIVEC_IMPLEMENTATION
#include <olive.c>

#include <stdlib.h>
#include <assert.h>

ArgonUI* argon_create(uint32_t* buffer, int width, int height, int stride) {
  ArgonUI* ar = malloc(sizeof(ArgonUI));
  ar->buffer = buffer;
  ar->canvas = olivec_canvas(buffer, width, height, stride);
  ar->input_state.press_position = (arPosition){0};
  ar->input_state.events = arDispatchedEventArray_create();
  argon_setContext(ar);
  return ar;
}

void argon_destroy(ArgonUI* self) {
  arDispatchedEventArray_destroy(self->input_state.events);
  free(self);
}

void argon_resize(ArgonUI* self, uint32_t* buffer, int width, int height, int stride) {
  self->buffer = buffer;
  self->canvas = olivec_canvas(buffer, width, height, stride);
}

void argon_draw(ArgonUI* argon, arView* root) {
  arView_draw(root, argon->canvas, (arPosition){0, 0});
}

static void _argon_handleEvent(ArgonUI* ui, arView* view, arPosition parent_pos) {
  arPosition pos = arPosition_add(view->current_pos, parent_pos);
  arEvent_handleAt(ui, view, pos, (arSize){view->previous_canvas.width, view->previous_canvas.height});

  arView* child = view->children->first;
  while (child != NULL) {
    _argon_handleEvent(ui, child, pos);
    child = child->next_sibling;
  }
}

void argon_handleEvents(ArgonUI* self, arView* root) {
  assert(root->parent == NULL);

  if (self->input_state.events->count == 0)
    return;

  _argon_handleEvent(self, root, (arPosition){0,0});

  argon_clearEvents(self);
}

static ArgonUI* ctx;

void argon_setContext(ArgonUI* ui) {
  ctx = ui;
}

ArgonUI* argon_getCurrentContext() {
  return ctx;
}

arPosition arPosition_add(arPosition a, arPosition b) {
  return (arPosition){a.x+b.x, a.y+b.y};
}

