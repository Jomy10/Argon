#define OLIVEC_IMPLEMENTATION
#include "../include/argon.h"

#include <stdlib.h>
#include <string.h>

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
  bool wrap;
} arTextData;

static void _arText_draw(arView* self, Olivec_Canvas canvas) {
  arTextData* data = (arTextData*)self->data;
  switch (data->font.type) {
    case AR_FONT_BITMAP:
      if (data->wrap) {
        arSize glyph_size = arText_getGlyphSize(self);
        size_t text_len = strlen(data->text);
        if (glyph_size.w * text_len > canvas.width) {
          size_t glyphs_per_line = canvas.width / glyph_size.w;
          char text[glyphs_per_line + 1];
          text[glyphs_per_line] = '\0';
          int i = 0;
          int next_i = 0;
          int y = 0;
          do {
            next_i += glyphs_per_line;
            if (next_i > text_len) {
              int l = next_i - (next_i - text_len);
              memcpy(text, data->text + i, l);
              text[l] = '\0';
            } else {
              memcpy(text, data->text + i, glyphs_per_line);
            }
            olivec_text(canvas, text, 0, y, data->font.value.bitmap_font, data->font.size, data->font.color);
            i = next_i;
            y += glyph_size.h;
          } while (i < text_len);
          return;
        } 
      }
      // default:
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

arView* arText_create(char* text, arFont font, bool shouldDeallocateText, bool enableWrapping) {
  arView* view = arView_create();
  arTextData* data = malloc(sizeof(arTextData));
  data->deallocateText = shouldDeallocateText;
  data->text = text;
  data->font = font;
  data->wrap = enableWrapping;
  view->data = data;
  view->draw = _arText_draw;
  view->destroy = _arText_destroy;

  return view;
}

void arText_setText(arView* self, char* text) {
  self->should_rerender = true;
  if (
    ((arTextData*)self->data)->deallocateText &&
    ((arTextData*)self->data)->text != NULL
  ) {
    free(((arTextData*)self->data)->text);
  }
  ((arTextData*)self->data)->text = text;
}

char* arText_getText(arView* self) {
  return ((arTextData*)self->data)->text;
}

void arText_setBitmapFont(arView* self, arBitmapFont font) {
  self->should_rerender = true;
  ((arTextData*)self->data)->font.type = AR_FONT_BITMAP;
  ((arTextData*)self->data)->font.value.bitmap_font = font;
}

void arText_setColor(arView* self, arColor color) {
  self->should_rerender = true;
  ((arTextData*)self->data)->font.color = color;
}

arColor arText_getColor(arView* self) {
  return ((arTextData*)self->data)->font.color;
}

void arText_setSize(arView* self, size_t size) {
  self->should_rerender = true;
  ((arTextData*)self->data)->font.size = size;
}

arSize arText_getGlyphSize(arView* self) {
  arTextData* data = (arTextData*) self->data;
  arSize size;
  switch (data->font.type) {
    case AR_FONT_BITMAP: {
      arBitmapFont* font = &data->font.value.bitmap_font;
      size.w = font->width * data->font.size;
      size.h = font->height * data->font.size;
    } break;
  }
  return size;
}

arSize arText_getSize(arView* self) {
  arTextData* data = (arTextData*) self->data;
  arSize size;
  switch (data->font.type) {
    case AR_FONT_BITMAP: {
      arBitmapFont* font = &data->font.value.bitmap_font;
      size.w = font->width * data->font.size;
      size.h = font->height * data->font.size;
      size.w = strlen(data->text) * size.w;
    } break;
  }
  return size;
}

