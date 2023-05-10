#ifndef CORE_CONTEXT_CONTEXT_H
#define CORE_CONTEXT_CONTEXT_H

#include "core/util/color.h"

#define PAINTED_EVENT 1
#define EXPOSE_EVENT 2

#define PAINT_HISTORY_SIZE 1024

typedef struct CommandContext {
  char command;
} CommandContext_t;

typedef struct Point {
  int x;
  int y;
} Point_t;

typedef struct PaintHistory {
  RGBColor_t rgb_color;
  Point_t start_point;
  Point_t end_point;
} PaintHistory_t;

typedef struct EventContext {
  int event_type;
  PaintHistory_t all_paint_histories[PAINT_HISTORY_SIZE];
  PaintHistory_t additional_paint_histories[PAINT_HISTORY_SIZE];
} EventContext_t;

typedef struct SocketContext {
  CommandContext_t command_context;
  EventContext_t event_context;
} SocketContext_t;

#endif
