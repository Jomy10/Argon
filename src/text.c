#define OLIVEC_IMPLEMENTATION
#include "../include/argon.h"

#include <stdlib.h>

arFont arFont_newBitmap(arBitmapFont bitmap_font, size_t size, arColor color) {
  arFont font;
  font.type = AR_FONT_BITMAP;
  font.value.bitmap_font = bitmap_font;
  font.size = size;
  font.color = color;
  return font;
}

typedef struct {
  arFont font;
  char* text;
  bool deallocateText;
} arTextData;

static void _arText_draw(arView* self, Olivec_Canvas canvas) {
  arTextData* data = (arTextData*)self->data;
  switch (data->font.type) {
    case AR_FONT_BITMAP:
      olivec_text(canvas, data->text, 0, 0, data->font.value.bitmap_font, data->font.size, data->font.color);
      break;
  }
}

static void _arText_destroy(arView* self) {
  if (((arTextData*)self->data)->deallocateText &&
    ((arTextData*)self->data)->text != NULL
  ) {
    free(((arTextData*)self->data)->text);
  }
}

arView* arText_create(char* text, arFont font, bool shouldDeallocateText) {
  arView* view = arView_create();
  arTextData* data = malloc(sizeof(arTextData));
  data->deallocateText = shouldDeallocateText;
  data->text = text;
  data->font = font;
  view->data = data;
  view->draw = _arText_draw;
  view->destroy = _arText_destroy;

  return view;
}

void arText_setText(arView* self, char* text) {
  self->destroy(self); // deallocate text
  ((arTextData*)self->data)->text = text;
}

void arText_setBitmapFont(arView* self, arBitmapFont font) {
  ((arTextData*)self->data)->font.type = AR_FONT_BITMAP;
  ((arTextData*)self->data)->font.value.bitmap_font = font;
}

void arText_setColor(arView* self, arColor color) {
  ((arTextData*)self->data)->font.color = color;
}
void arText_setSize(arView* self, size_t size) {
  ((arTextData*)self->data)->font.size = size;
}

