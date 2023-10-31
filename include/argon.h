//
// Argon
// =====
// A simple UI library that renders to a framebuffer for C and C++.
//
// Copyright (C) 2023  Jonas Everaert
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
// 

#ifndef _ARGON_H
#define _ARGON_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <olive.c>

typedef uint32_t arColor; 

struct _arChildrenList;

//struct _arvec;
typedef struct _arView {
  void* data;
  struct _arView* parent;
  struct _arChildrenList* children;
  bool should_rerender;
  Olivec_Canvas previous_canvas;
  void (*draw)(struct _arView* self, Olivec_Canvas canvas);
  /// nullable
  void (*destroy)(struct _arView* self);
} arView;

// Create a default arView.
// The default arView works as a generic container for UI elements
arView* arView_create();
void arView_destroy(arView* self);
void arView_draw(arView* self, Olivec_Canvas);
void arView_addChild(arView* self, arView* child);

typedef struct _arUI {
  uint32_t* buffer;
  Olivec_Canvas canvas;
} ArgonUI;

ArgonUI argon_init(uint32_t* buffer, int w, int h, int stride);
void argon_draw(ArgonUI*, arView* root);

// views //
arView* arFill_create(arColor);
void arFill_setColor(arView* self, arColor);

arView* arContainer_create(size_t w, size_t h);
void arContainer_setWidth(arView*, size_t);
void arContainer_setHeight(arView*, size_t);

arView* arHStack_create();
void arHStack_setWidth(arView* self, int at_index, int width);
void arHStack_assureMinWidthCap(arView* self, int cap);

arView* arVStack_create();
void arVStack_setHeight(arView* self, int at_index, int width);
void arVStack_assureMinHeightCap(arView* self, int cap);


struct _arChildrenList {
  int size;
  arView** values;
  int cap;
};

#ifdef __cplusplus
}
#endif

#endif

