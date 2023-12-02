#include "../include/argon.h"
#include "children_list.h"

#include <stdlib.h>

void arView_draw(arView* self, Olivec_Canvas canvas, arPosition at) {
  self->current_pos = at;
  self->previous_canvas = canvas;
  if (self->should_rerender) {
    // for (int i = 0; i < self->children->size; i++) {
    //   self->children->values[i]->should_rerender = true;
    // }
    arView* child = self->children->first;
    while (child != NULL) {
      child->should_rerender = true;
      child = child->next_sibling;
    }
    self->draw(self, canvas);
    self->should_rerender = false;
  } else {
    arView* child = self->children->first;
    while (child != NULL) {
      arView_draw(child, child->previous_canvas, child->current_pos);
      child = child->next_sibling;
    }
  }
}

static void _arView_default_draw_cb(arView* self, Olivec_Canvas canvas) {
  arView* child = self->children->first;
  while (child != NULL) {
    arView_draw(child, canvas, self->current_pos);
    child = child->next_sibling;
  }
}

arView* arView_create() {
  arView* view = malloc(sizeof(arView));
  view->data = NULL;
  view->children = childrenList_create();
  view->next_sibling = NULL;
  view->draw = _arView_default_draw_cb;
  view->should_rerender = true;
  view->parent = NULL;
  view->destroy = NULL;
  view->previous_canvas = OLIVEC_CANVAS_NULL;
  view->current_pos = (arPosition){0,0};
  view->ui = argon_getCurrentContext();
  view->onClick = NULL;
  #ifdef ARGON_MANAGE_CHILDREN_MANUALLY
  view->manual_children_management_callback = NULL;
  #endif
  #ifdef ARGON_USER_DATA
  view->user_data = NULL;
  #endif

  return view;
}

void arView_destroy(arView* self) {
  // free data
  if (self->destroy != NULL)
    self->destroy(self);
  if (self->data != NULL)
    free(self->data);

  arView* child = self->children->first;
  arView* child_to_destroy;
  while (child != NULL) {
    #ifndef ARGON_MANAGE_CHILDREN_MANUALLY
    child_to_destroy = child;
    child = child_to_destroy->next_sibling;
    arView_destroy(child_to_destroy);
    #else
    if (child->manual_children_management_callback != NULL)
      child->manual_children_management_callback(child->manual_children_management_data);
    child = child->next_sibling;
    #endif
  }
  childrenList_destroy(self->children);

  free(self);
}

void arView_addChild(arView* self, arView* child) {
  childrenList_add(self->children, child);
  child->parent = self;

  //arvec_append(self->children, &child);
  self->should_rerender = true;
}

void arView_rmChild(arView* self, arView* child) {
  childrenList_remove(self->children, child);
  child->parent = NULL;
  self->should_rerender = true;
}

void arView_clearChildren(arView* self) {
  arView* child = self->children->first;
  while (child != NULL) {
    childrenList_remove(self->children, child);
    child->parent = NULL;
    child = child->next_sibling;
  }
  self->should_rerender = true;
}

void arView_setOnClick(arView *self, void (*onClick)(arView* self)) {
  self->onClick = onClick;
}

