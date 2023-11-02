#include "event.h"

#include  <stdlib.h>

#include "../include/argon.h"

void argon_dispatchEvent(ArgonUI* state, arEvent ev) {
  switch (ev.type) {
    case AR_EV_POINTER_PRESS:
      state->input_state.press_position = ev.data.pointer_pos;
      break;
    case AR_EV_POINTER_RELEASE:
      if (state->input_state.events->count == state->input_state.events->cap) {
        arDispatchedEventArray_assureMinCap(
          state->input_state.events,
          state->input_state.events->cap == 0 
            ? 10
            : state->input_state.events->cap * 2);
      }
    
      arDispatchedEvent disp_ev = (arDispatchedEvent){
          .type = AR_DISPEV_CLICK,
          .data.click = {
            .start_position = state->input_state.press_position,
            .end_position = ev.data.pointer_pos,
          },
        };

      state->input_state.events->values[state->input_state.events->count] = disp_ev;
      state->input_state.events->count += 1;

      break;
  }
}

void argon_clearEvents(ArgonUI* self) {
  self->input_state.events->count = 0;
}

inline void arView_onClick(arView* self) {
  self->onClick(self);
}

static inline bool _box_point_collision(arPosition box_start, arSize box_size, arPosition point) {
  return (point.x > box_start.x && point.y > box_start.y
    && point.x < box_start.x + box_size.w && point.y < box_start.y + box_size.h);
}

void arEvent_handleAt(ArgonUI* state, arView* view, arPosition pos, arSize size) {
  if (view->onClick == NULL) return;

  int c = state->input_state.events->count;
  arDispatchedEvent* ev;
  for (int i = 0; i < c; i++) {
    ev = &state->input_state.events->values[i];
    switch (ev->type) {
      case AR_DISPEV_CLICK:
        if (
          _box_point_collision(pos, size, ev->data.click.start_position) &&
          _box_point_collision(pos, size, ev->data.click.end_position)
        ) {
          arView_onClick(view);
        }
        break;
    }
  }
}

#include "array.h"

AR_DEF_ARRAY(arDispatchedEventArray, arEvent);

