#include <SDL2/SDL.h>
#include "SDL_pixels.h"
#include "SDL_video.h"
#include "include/argon.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
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

#define WIDTH 800
#define HEIGHT 400

int main(int argc, char** argv) {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* window = SDL_CreateWindow("test",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE
  );

  SDL_Surface* window_surface = SDL_GetWindowSurface(window);
  uint32_t* buffer = window_surface->pixels;
  // should be SDL_PIXELFORMAT_ARGB8888
  //printf("Pixel format: %s\n", SDL_GetPixelFormatName(window_surface->format->format));

  char* example;
  if (argc == 0) {
    example = "test";
  } else {
    example = argv[0];
  }
  // TODO: render correct example

  ArgonUI* ui = argon_create(buffer, WIDTH, HEIGHT, WIDTH);

  arView* root = arView_create();
  arView* container = arContainer_create(100, HEIGHT);
  arView_addChild(root, container);
  arView* fill = arFill_create(0xFFFF0000);
  arFont font = arFont_newBitmap((arBitmapFont)olivec_default_font, 4, 0xFF00FF00);
  arView* arText = arText_create("hello world", font, false, true);
  arView_addChild(container, fill);
  arView_addChild(container, arText);

  while (true) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        return 0;
      if (event.type == SDL_WINDOWEVENT) {
        if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
          window_surface = SDL_GetWindowSurface(window);
          buffer = window_surface->pixels;
          argon_resize(ui, buffer, window_surface->w, window_surface->h, window_surface->w);
          memset(buffer, 0, window_surface->w * window_surface->h * 4);
          root->should_rerender = true;
        }
      }
    }

    argon_handleEvents(ui, root);
    argon_draw(ui, root);

    SDL_UpdateWindowSurface(window);
  }

  arView_destroy(root);
  argon_destroy(ui);
  SDL_DestroyWindow(window);
}

/*
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

  arView* fillContainer = arContainer_create(10, 100);
  //arView_setOnClick(fillContainer, container_onClick);
  //arContainer_setMargin(fillContainer, (arMargin){10, 30, 30, 30});
  arView* fill = arFill_create(0xFF0F0FFF);
  uint32_t fontColor = 0xFFFF0000;
  arFont font = arFont_newBitmap(olivec_default_font, 2, fontColor);
  arView* fillText = arText_create("hello world", font, false, true);
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
    // arFill_setColor(fill, getColor());
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
*/
