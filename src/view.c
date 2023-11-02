#include "../include/argon.h"
#include "vec.h"

#include <stdlib.h>

typedef struct _arChildrenList ChildrenList;

// TODO: linked list?
static ChildrenList* childrenList_create() {
  ChildrenList* list = malloc(sizeof(ChildrenList));
  list->size = 0;
  list->cap = 0;
  list->values = NULL;

  return list;
}

static void childrenList_destroy(ChildrenList* self) {
  free(self->values);
  free(self);
}

static void childrenList_assureMinCap(ChildrenList* self, int cap) {
  if (self->cap < cap) {
    if (self->cap == 0) {
      self->cap = cap;
      self->values = malloc(cap * sizeof(arView*));
    } else {
      self->cap = cap;
      self->values = realloc(self->values, cap * sizeof(arView*));
    }
  }
}

void arView_draw(arView* self, Olivec_Canvas canvas, arPosition at) {
  self->current_pos = at;
  self->previous_canvas = canvas;
  if (self->should_rerender) {
    for (int i = 0; i < self->children->size; i++) {
      self->children->values[i]->should_rerender = true;
    }
    self->draw(self, canvas);
    self->should_rerender = false;
  } else {
    for (int i = 0; i < self->children->size; i++) {
      arView* child = self->children->values[i];
      arView_draw(child, child->previous_canvas, child->current_pos);
    }
  }
}

static void _arView_default_draw_cb(arView* self, Olivec_Canvas canvas) {
  for (int i = 0; i < self->children->size; i++) {
    arView* child = self->children->values[i];
    arView_draw(child, canvas, self->current_pos);
  }
}

arView* arView_create() {
  arView* view = malloc(sizeof(arView));
  view->data = NULL;
  view->children = childrenList_create();
  view->draw = _arView_default_draw_cb;
  view->should_rerender = true;
  view->parent = NULL;
  view->destroy = NULL;
  view->previous_canvas = OLIVEC_CANVAS_NULL;
  view->current_pos = (arPosition){0,0};
  view->ui = argon_getCurrentContext();
  view->onClick = NULL;

  return view;
}

void arView_destroy(arView* self) {
  // free data
  if (self->destroy != NULL)
    self->destroy(self);
  if (self->data != NULL)
    free(self->data);

  for (int i = 0; i < self->children->size; i++) {
    arView_destroy(self->children->values[i]);
  }
  childrenList_destroy(self->children);

  free(self);
}

void arView_addChild(arView* self, arView* child) {
  if (self->children->size == self->children->cap)
    childrenList_assureMinCap(self->children, self->children->cap == 0 ? 3 : self->children->cap * 2);
  self->children->values[self->children->size++] = child;
  child->parent = self;

  //arvec_append(self->children, &child);
  self->should_rerender = true;
}

void arView_setOnClick(arView *self, void (*onClick)(arView* self)) {
  self->onClick = onClick;
}

