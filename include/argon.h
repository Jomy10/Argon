///
/// Argon
/// =====
/// @brief A simple UI library that renders to a framebuffer for C and C++.
/// @date 31 Oct 2023
/// @file argon.h
/// @author Jonas Everaert
///
/// Copyright (C) 2023  Jonas Everaert
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU Lesser General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU Lesser General Public License for more details.
///
/// You should have received a copy of the GNU Lesser General Public License
/// along with this program.  If not, see <https://www.gnu.org/licenses/>.
/// 
///
/// ## Usage
///
/// Functions in the form of `[...]_create` create a new object that should be
/// deallocated using `[...]_destroy`. Functions in the form of `[...]_new` create
/// objects that shouldn't be deallocated.
///
/// **-DARGON_MANAGE_CHILDREN_MANUALLY**
/// meant for interop with memory managed languages (see swift bindings)
///

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

// TODO: reducing struct size to reduce allocations
/// @ingroup arView
typedef struct _arView {
  void* data;
  struct _arView* parent;
  struct _arView* next_sibling;
  struct _arChildrenList* children;
  bool should_rerender;
  Olivec_Canvas previous_canvas;
  struct _arUI* ui;
  /// Position within the parent container
  arPosition current_pos;
  void (*draw)(struct _arView* self, Olivec_Canvas canvas);
  /// nullable
  void (*destroy)(struct _arView* self);
  void (*onClick)(struct _arView* self);
  #ifdef ARGON_MANAGE_CHILDREN_MANUALLY
  void* manual_children_management_data;
  void (*manual_children_management_callback)(void* self);
  #endif
} arView;

// Argon //
/// @defgroup ArgonUI ArgonUI Context
/// @{
typedef struct _arUI {
  uint32_t* buffer;
  Olivec_Canvas canvas;
  struct {
    arPosition press_position;
    struct _arDispatchedEventArray* events;
  } input_state;
} ArgonUI;

/// Create a new ArgonUI context and set it as the current context
ArgonUI* argon_create(uint32_t* buffer, int w, int h, int stride);
void argon_destroy(ArgonUI*);
/// to redraw the UI after resize, call root->should_rerender = true
void argon_resize(ArgonUI* ui, uint32_t* buffer, int w, int h, int stride);
void argon_draw(ArgonUI*, arView* root);
void argon_handleEvents(ArgonUI*, arView*);
/// Set the context to be used for any [view]_create() calls
void argon_setContext(ArgonUI*);
ArgonUI* argon_getCurrentContext();
void argon_swapBuffers(ArgonUI* self, arView* root, uint32_t* new_buffer);
/// @}

typedef struct {
  int l, r, u, d;
} arMargin;

// Events //
/// @defgroup Events
/// @{
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

// TODO: how to get this function to display in 2 groups with doxygen?
/// @ingroup ArgonUI Events
void argon_dispatchEvent(ArgonUI* state, arEvent event);
/// @}

// Fonts //
/// @defgroup Fonts
/// @{
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

/// Create a new arFont from a bitmap font definition
/// @see [olive.c](https://github.com/tsoding/olive.c/blob/master/olive.c#L41-L44) for an example
arFont arFont_newBitmap(arBitmapFont, size_t size, arColor color);
/// @}

// views //
/// @defgroup Views 
/// Functions relating to all arView options
/// @{

/// @defgroup arView
/// @{
/// Create a default arView.
/// The default arView works as a generic container for UI elements
arView* arView_create();
void arView_destroy(arView* self);
//// Draw a view into a canvas
///
/// @param at[in] coordinates in the parent's canvas (or the global canvas in case
///               of the root view), not used for actual drawing, but for handling
///               input events
void arView_draw(arView* self, Olivec_Canvas, arPosition at);
void arView_addChild(arView* self, arView* child);
/// This method will not destory the child, but simply removes it from its parent
void arView_rmChild(arView* self, arView* child);
/// Removes all children from the view, these children should be destroyed if
/// you no longer need them
void arView_clearChildren(arView* self);
void arView_setOnClick(arView* self, void (*onClick)(arView* self));
/// @}

/// @defgroup arFill
/// @{
arView* arFill_create(arColor);
void arFill_setColor(arView* self, arColor);
arColor arFill_getColor(arView* self);
/// @}

/// @defgroup arContainer
/// @{
arView* arContainer_create(size_t w, size_t h);
void arContainer_setWidth(arView*, size_t);
void arContainer_setHeight(arView*, size_t);
void arContainer_setMargin(arView*, arMargin);
arMargin arContainer_getMargin(arView*);
/// @}

/// @defgroup arHStack
/// @{
arView* arHStack_create();
void arHStack_setWidth(arView* self, int at_index, int width);
void arHStack_assureMinWidthCap(arView* self, int cap);
/// @}

/// @defgroup arVStack
/// @{
arView* arVStack_create();
void arVStack_setHeight(arView* self, int at_index, int width);
void arVStack_assureMinHeightCap(arView* self, int cap);
/// @}

/// @defgroup arText
/// @{
arView* arText_create(char* text, arFont font, bool shouldDeallocateText, bool enableWrapping);
// Get the size of the whole text
arSize arText_getSize(arView* self);
// Get the size of a single glyph
arSize arText_getGlyphSize(arView* self);
void arText_setText(arView* self, char* text);
void arText_setBitmapFont(arView* self, arBitmapFont font);
void arText_setColor(arView* self, arColor color);
void arText_setSize(arView* self, size_t size);
/// @}

/// @defgroup arCanvas
/// @{
arView* arCanvas_create(size_t width, size_t height);
arView* arCanvas_createWithData(uint32_t* data, size_t width, size_t height);
arView* arCanvas_createWithCanvas(Olivec_Canvas canvas);
/// to redraw the canvas, call self->should_rerender = true;
Olivec_Canvas arCanvas_getCanvas(arView* self);
/// to redraw the canvas, call self->should_rerender = true;
uint32_t* arCanvas_getData(arView* self);
/// @}

/// @defgroup arSubCanvas
/// @{
/// Creates a canvas that renders directly to the frame buffer
///
/// Use with caution! This kind of canvas should only be used 
/// for performance reasons
arView* arSubCanvas_create(size_t width, size_t height);
/// Don't forget to take the stride into account when drawing to this kind of canvas.
/// This canvas should also be renewed after every UI update!
Olivec_Canvas arSubCanvas_getCanvas(arView* self);
/// @}

// TODO: dynamic canvas (variable size/width)

/// @}

/// @defgroup ChildrenList arView Children
/// @{
struct _arChildrenList {
  arView* first;
  arView* last;
};

typedef struct _arChildrenList ARChildrenList;

ARChildrenList* childrenList_create();

void childrenList_destroy(ARChildrenList* self);

void childrenList_add(ARChildrenList* self, arView* child);

void childrenList_remove(ARChildrenList* self, arView* child);
/// @}

#ifdef __cplusplus
}
#endif

#endif

