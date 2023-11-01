#include <MiniFB.h>
#include "include/argon.h"
#include <stdlib.h>
#include <stdio.h>

#define OLIVEC_IMPLEMENTATION
#include "olive.c"

int main() {
  struct mfb_window* window = mfb_open("test", 800, 400);
  if (!window) return 1;

  uint32_t* buffer = (uint32_t*) malloc(800 * 400 * 4);

  //Olivec_Canvas canvas = olivec_canvas(buffer, 800, 400, 800);
  ArgonUI ui = argon_init(buffer, 800, 400, 800);

  arView* root = arView_create();

  arView* container = arContainer_create(400, 100);
  arView_addChild(root, container);

  arView* hstack = arHStack_create();
  arHStack_setWidth(hstack, 1, 100);
  arHStack_setWidth(hstack, 0, 200);
  arHStack_setWidth(hstack, 2, 100);
  arView_addChild(container, hstack);

  arView* fillContainer = arContainer_create(200, 100);
  arView* fill = arFill_create(0xFF0000FF);
  arFont font = arFont_newBitmap(olivec_default_font, 2, 0xFFFFF0FF);
  arView* fillText = arText_create("hello world", font, false);
  arView_addChild(fillContainer, fill);
  arView_addChild(fillContainer, fillText);
  arView_addChild(hstack, fillContainer);

  arView* fill1 = arFill_create(0x0000ffF0);
  arView_addChild(hstack, fill1);

  arView* fill2 = arFill_create(0x00ff00FF);
  arView_addChild(hstack, fill2);

  do {
    argon_draw(&ui, root);
    int state = mfb_update(window, buffer);
  } while(mfb_wait_sync(window));
}

