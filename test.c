#include <MiniFB.h>
#include "include/argon.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#define OLIVEC_IMPLEMENTATION
#include "olive.c"

arColor getColor() {
  static int i = 0;
  i++;
  if (i == 3) i = 0;
  switch (i) {
    case 0: return 0xFF0000FF;
    case 1: return 0xFF00FFFF;
    case 2: return 0xFFFF00FF;
    default: exit(1);
  }
}

void container_onClick(arView* self) {
  printf("Auwch, that hurt!\n");
}

int main() {
  struct mfb_window* window = mfb_open_ex("test", 800, 400, WF_RESIZABLE);
  if (!window) return 1;

  uint32_t* buffer = (uint32_t*) malloc(800 * 400 * 4);

  //Olivec_Canvas canvas = olivec_canvas(buffer, 800, 400, 800);
  ArgonUI* ui = argon_create(buffer, 800, 400, 800);

  arView* root = arView_create();
  assert(root->parent == NULL);

  arView* container = arContainer_create(400, 100);
  arView_addChild(root, container);
  arView_setOnClick(container, container_onClick);

  arView* hstack = arHStack_create();
  arHStack_setWidth(hstack, 1, 100);
  arHStack_setWidth(hstack, 0, 200);
  arHStack_setWidth(hstack, 2, 100);
  arView_addChild(container, hstack);

  arView* fillContainer = arContainer_create(200, 100);
  //arView_setOnClick(fillContainer, container_onClick);
  //arContainer_setMargin(fillContainer, (arMargin){10, 30, 30, 30});
  arView* fill = arFill_create(0xFF0000FF);
  uint32_t fontColor = 0xFFFF0000;
  arFont font = arFont_newBitmap(olivec_default_font, 2, fontColor);
  arView* fillText = arText_create("hello world", font, false);
  arView_addChild(fillContainer, fill);
  arView_addChild(fillContainer, fillText);
  arView_addChild(hstack, fillContainer);

  arView* fill1 = arFill_create(0x0000ffF0);
  arView_addChild(hstack, fill1);

  // arView* fill2 = arFill_create(0x00ff00FF);
  // arView_addChild(hstack, fill2);
  arView* canvas = arCanvas_create(100, 100);
  arView_addChild(hstack, canvas);
  uint32_t* canvas_data = arCanvas_getData(canvas);
  // for (int i = 0; i < 100 * 100; i++) {
  //   canvas_data[i] = rand();
  // }

  argon_dispatchEvent(ui, (arEvent){AR_EV_POINTER_PRESS, (arEventData){.pointer_pos = (arPosition){10, 10} }});
  argon_dispatchEvent(ui, (arEvent){AR_EV_POINTER_RELEASE, (arEventData){.pointer_pos = (arPosition){10, 15} }});

  do {
    for (int i = 0; i < 100 * 100; i++) {
      canvas_data[i] = rand();
    }
    canvas->should_rerender = true;
    arFill_setColor(fill, getColor());
    argon_draw(ui, root);
    argon_handleEvents(ui, root);
    //printf("UI drawn %u in %lu->%lu @ %p\n", buffer[0], ui.canvas.width, ui.canvas.height, ui.canvas.pixels);
    int state = mfb_update_ex(window, ui->buffer, 800, 400);
    if (state < 0) {
      window = NULL;
      break;
    }
  } while(mfb_wait_sync(window));

  arView_destroy(root);
  argon_destroy(ui);
}

