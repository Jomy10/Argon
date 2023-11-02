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

//
// # Usage
//
// Functions in the form of `[...]_create` create a new object that should be
// deallocated using `[...]_destroy`. Functions in the form of `[...]_new` create
// objects that shouldn't be deallocated.
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
struct _arDispatchedEventArray;
struct _arUI;

typedef struct {
  size_t x, y;
} arPosition;

typedef struct {
  size_t w, h;
} arSize;

arPosition arPosition_add(arPosition, arPosition);

typedef struct _arView {
  void* data;
  struct _arView* parent;
  struct _arChildrenList* children;
  bool should_rerender;
  Olivec_Canvas previous_canvas;
  struct _arUI* ui;
  // Position within the parent container
  arPosition current_pos;
  void (*draw)(struct _arView* self, Olivec_Canvas canvas);
  // nullable
  void (*destroy)(struct _arView* self);
  void (*onClick)(struct _arView* self);
} arView;

typedef struct _arUI {
  uint32_t* buffer;
  Olivec_Canvas canvas;
  struct {
    arPosition press_position;
    struct _arDispatchedEventArray* events;
  } input_state;
} ArgonUI;

// Argon //
// Create a new ArgonUI context and set it as the current context
ArgonUI* argon_create(uint32_t* buffer, int w, int h, int stride);
void argon_destroy(ArgonUI*);
// to redraw the UI after resize, call root->should_rerender = true
void argon_resize(ArgonUI* ui, uint32_t* buffer, int w, int h, int stride);
void argon_draw(ArgonUI*, arView* root);
void argon_handleEvents(ArgonUI*, arView*);
// Set the context to be used for any [view]_create() calls
void argon_setContext(ArgonUI*);
ArgonUI* argon_getCurrentContext();

typedef struct {
  int l, r, u, d;
} arMargin;

// Events //
typedef enum {
  AR_EV_POINTER_PRESS,
  AR_EV_POINTER_RELEASE,
} arEventType;

typedef union {
  arPosition pointer_pos;
} arEventData;

typedef struct {
  arEventType type;
  arEventData data;
} arEvent;

void argon_dispatchEvent(ArgonUI* state, arEvent event);

// views //
// Create a default arView.
// The default arView works as a generic container for UI elements
arView* arView_create();
void arView_destroy(arView* self);
// - at [arPosition]: coordinates in the global canvas, not used for actual drawing,
//   but for handling input events
void arView_draw(arView* self, Olivec_Canvas, arPosition at);
void arView_addChild(arView* self, arView* child);
void arView_setOnClick(arView* self, void (*onClick)(arView* self));

arView* arFill_create(arColor);
void arFill_setColor(arView* self, arColor);

arView* arContainer_create(size_t w, size_t h);
void arContainer_setWidth(arView*, size_t);
void arContainer_setHeight(arView*, size_t);
void arContainer_setMargin(arView*, arMargin);
arMargin arContainer_getMargin(arView*);

arView* arHStack_create();
void arHStack_setWidth(arView* self, int at_index, int width);
void arHStack_assureMinWidthCap(arView* self, int cap);

arView* arVStack_create();
void arVStack_setHeight(arView* self, int at_index, int width);
void arVStack_assureMinHeightCap(arView* self, int cap);

typedef Olivec_Font arBitmapFont;
enum FontType {
  AR_FONT_BITMAP,
};

union FontValue {
  arBitmapFont bitmap_font;
};

struct _arFont {
  enum FontType type;
  union FontValue value;
  size_t size;
  arColor color;
};
typedef struct _arFont arFont;
arFont arFont_newBitmap(arBitmapFont, size_t size, arColor color);
arView* arText_create(char* text, arFont font, bool shouldDeallocateText, bool enableWrapping);
// Get the size of the whole text
arSize arText_getSize(arView* self);
// Get the size of a single glyph
arSize arText_getGlyphSize(arView* self);
void arText_setText(arView* self, char* text);
void arText_setBitmapFont(arView* self, arBitmapFont font);
void arText_setColor(arView* self, arColor color);
void arText_setSize(arView* self, size_t size);

arView* arCanvas_create(size_t width, size_t height);
arView* arCanvas_createWithData(uint32_t* data, size_t width, size_t height);
arView* arCanvas_createWithCanvas(Olivec_Canvas canvas);
// to redraw the canvas, call self->should_rerender = true;
Olivec_Canvas arCanvas_getCanvas(arView* self);
// to redraw the canvas, call self->should_rerender = true;
uint32_t* arCanvas_getData(arView* self);

// TODO: dynamic canvas (variable size/width) & SubCanvas (draw directly onto framebuffer)

struct _arChildrenList {
  int size;
  arView** values;
  int cap;
};

#ifdef __cplusplus
}
#endif

#endif

