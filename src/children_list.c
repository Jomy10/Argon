#include "children_list.h"
#include <stdlib.h>

ARChildrenList* childrenList_create() {
  ARChildrenList* list = malloc(sizeof(ARChildrenList));
  list->first = NULL;
  list->last = NULL;

  return list;
}

void childrenList_destroy(ARChildrenList* self) {
  free(self);
}

void childrenList_add(ARChildrenList* self, arView* child) {
  if (self->first == NULL) {
    self->first = child;
    self->last = child;
  } else {
    self->last->next_sibling = child;
    self->last = child;
  }
}

void childrenList_remove(ARChildrenList* self, arView* child) {
  arView* v;
  v = self->first;
  if (v == child) {
    self->first = self->first->next_sibling;
    if (self->last == child) {
      self->last = NULL;
    }
  }

  while (v->next_sibling != NULL) {
    if (v->next_sibling == child) {
      v->next_sibling = child->next_sibling;
      if (self->last == child) {
        self->last = v;
      }
      break;
    }
    v = v->next_sibling;
  }
}

