#define OLIVEC_IMPLEMENTATION
#include "../include/argon.h"

ArgonUI argon_init(uint32_t* buffer, int width, int height, int stride) {
  ArgonUI ar;
  ar.buffer = buffer;
  ar.canvas = olivec_canvas(buffer, width, height, stride);
  return ar;
}

void argon_draw(ArgonUI* argon, arView* root) {
  arView_draw(root, argon->canvas);
}

