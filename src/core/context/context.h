#ifndef CORE_CONTEXT_CONTEXT_H
#define CORE_CONTEXT_CONTEXT_H

#include "core/util/color.h"

#define GET_REQUEST_METHOD 0
#define POST_REQUEST_METHOD 1

#define PAINTED_EVENT 1
#define EXPOSE_EVENT 2

#define MESSAGE_LENGTH 128
#define MESSAGE_HISTORY_SIZE 128
#define PAINT_HISTORY_SIZE 1024

typedef struct MessageHistory {
  int client_id;
  RGBColor_t rgb_color;
  char message[MESSAGE_LENGTH];
} MessageHistory_t;

typedef struct MessageContext {
  MessageHistory_t all_message_histories[MESSAGE_HISTORY_SIZE];
  MessageHistory_t additional_message_history;
} MessageContext_t;

typedef struct CommandContext {
  char command;
  MessageContext_t message_context;
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
  PaintHistory_t additional_paint_history;
} EventContext_t;

typedef struct SocketContext {
  int request_method;
  CommandContext_t command_context;
  EventContext_t event_context;
} SocketContext_t;

#endif
