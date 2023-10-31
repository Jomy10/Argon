#include "../include/argon.h"
#include "vec.h"

#include <stdlib.h>

typedef struct _arChildrenList ChildrenList;

static ChildrenList* childrenList_create() {
  ChildrenList* list = malloc(sizeof(ChildrenList));
  list->size = 0;
  list->cap = 0;
  list->values = NULL;

  return list;
}

static void childrenList_free(ChildrenList* self) {
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

void arView_draw(arView* self, Olivec_Canvas canvas) {
  if (self->should_rerender) {
    self->draw(self, canvas);
  } else {
    for (int i = 0; i < self->children->size; i++) {
      arView* child = self->children->values[i];
      child->draw(child, child->previous_canvas);
    }
  }
  self->previous_canvas = canvas;
}

static void _arView_default_draw_cb(arView* self, Olivec_Canvas canvas) {
  for (int i = 0; i < self->children->size; i++) {
    arView* child = self->children->values[i];
    child->draw(child, canvas);
  }
}

static void _arView_default_destroy_cb(arView* self) {
  if (self->data != NULL)
    free(self->data);
}

arView* arView_create() {
  arView* view = malloc(sizeof(arView));
  view->data = NULL;
  view->children = childrenList_create();
  view->draw = _arView_default_draw_cb;
  view->should_rerender = true;
  view->parent = NULL;
  view->destroy = _arView_default_destroy_cb;

  return view;
}

void arView_destroy(arView* self) {
  if (self->destroy != NULL)
    self->destroy(self);

  childrenList_free(self->children);
  if (self->data != NULL)
    free(self->data);
}

void arView_addChild(arView* self, arView* child) {
  if (self->children->size == self->children->cap)
    childrenList_assureMinCap(self->children, self->children->cap == 0 ? 3 : self->children->cap * 2);
  self->children->values[self->children->size++] = child;
  self->parent = self;

  //arvec_append(self->children, &child);
  self->should_rerender = true;
}

