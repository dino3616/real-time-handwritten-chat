#ifndef MODULE_CONTEXT_CONTEXT_H
#define MODULE_CONTEXT_CONTEXT_H

#define PAINTED_EVENT 1
#define EXPOSE_EVENT 2

typedef struct Point {
  int x;
  int y;
} Point_t;

typedef struct PointHistory {
  Point_t start_point;
  Point_t end_point;
} PointHistory_t;

typedef struct EventContext {
  int event_type;
  PointHistory_t all_point_histories[256];
  PointHistory_t additional_point_histories[256];
} EventContext_t;

typedef struct SocketContext {
  char command;
  EventContext_t event_context;
} SocketContext_t;

#endif
