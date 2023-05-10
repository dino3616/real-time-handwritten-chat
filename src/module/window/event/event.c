#include "event.h"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>

#include "module/context/context.h"
#include "module/error/error.h"
#include "module/window/window.h"

int render_event_handler(WindowManager_t *window_manager, int socket_fd,
                         SocketContext_t *socket_context) {
  static XEvent event;
  static XPoint startPoint = {.x = 0, .y = 0};
  static PointHistory_t point_histories[256];

  if (XEventsQueued(window_manager->display, QueuedAfterFlush) == None) {
    return EXIT_SUCCESS;
  };

  XNextEvent(window_manager->display, &event);

  switch (event.type) {
    case ButtonPress: {
      bzero(point_histories, sizeof(PointHistory_t) * 256);

      startPoint.x = event.xbutton.x;
      startPoint.y = event.xbutton.y;

      break;
    }
    case ButtonRelease: {
      socket_context->event_context.event_type = PAINTED_EVENT;
      if (memcpy(socket_context->event_context.additional_point_histories,
                 point_histories, sizeof(PointHistory_t) * 256) == NULL) {
        int error_number = errno;
        print_error("Failed to copy point histories. cause: '%s'\n",
                    strerror(error_number));

        return EXIT_FAILURE;
      }

      send(socket_fd, socket_context, sizeof(SocketContext_t), 0);

      bzero(point_histories, sizeof(PointHistory_t) * 256);
      bzero(socket_context->event_context.additional_point_histories,
            sizeof(PointHistory_t) * 256);

      break;
    }
    case Expose: {
      socket_context->event_context.event_type = EXPOSE_EVENT;

      send(socket_fd, socket_context, sizeof(SocketContext_t), 0);

      break;
    }
    case MotionNotify: {
      XDrawLine(window_manager->display, window_manager->window,
                window_manager->gc, startPoint.x, startPoint.y, event.xmotion.x,
                event.xmotion.y);

      for (int i = 0; i < 256; i++) {
        if (point_histories[i].start_point.x == 0 &&
            point_histories[i].start_point.y == 0 &&
            point_histories[i].end_point.x == 0 &&
            point_histories[i].end_point.y == 0) {
          point_histories[i].start_point.x = startPoint.x;
          point_histories[i].start_point.y = startPoint.y;
          point_histories[i].end_point.x = event.xmotion.x;
          point_histories[i].end_point.y = event.xmotion.y;

          break;
        }
      }

      startPoint.x = event.xmotion.x;
      startPoint.y = event.xmotion.y;

      break;
    }
  }

  return EXIT_SUCCESS;
}

int rerender_event_handler(WindowManager_t *window_manager,
                           SocketContext_t *socket_context) {
  switch (socket_context->event_context.event_type) {
    case PAINTED_EVENT: {
      for (int i = 0; i < 256; i++) {
        if (socket_context->event_context.additional_point_histories[i]
                    .start_point.x == 0 &&
            socket_context->event_context.additional_point_histories[i]
                    .start_point.y == 0 &&
            socket_context->event_context.additional_point_histories[i]
                    .end_point.x == 0 &&
            socket_context->event_context.additional_point_histories[i]
                    .end_point.y == 0) {
          break;
        }

        XDrawLine(window_manager->display, window_manager->window,
                  window_manager->gc,
                  socket_context->event_context.additional_point_histories[i]
                      .start_point.x,
                  socket_context->event_context.additional_point_histories[i]
                      .start_point.y,
                  socket_context->event_context.additional_point_histories[i]
                      .end_point.x,
                  socket_context->event_context.additional_point_histories[i]
                      .end_point.y);
      }

      break;
    }
    case EXPOSE_EVENT: {
      for (int i = 0; i < 256; i++) {
        if (socket_context->event_context.all_point_histories[i]
                    .start_point.x == 0 &&
            socket_context->event_context.all_point_histories[i]
                    .start_point.y == 0 &&
            socket_context->event_context.all_point_histories[i].end_point.x ==
                0 &&
            socket_context->event_context.all_point_histories[i].end_point.y ==
                0) {
          break;
        }

        XDrawLine(
            window_manager->display, window_manager->window, window_manager->gc,
            socket_context->event_context.all_point_histories[i].start_point.x,
            socket_context->event_context.all_point_histories[i].start_point.y,
            socket_context->event_context.all_point_histories[i].end_point.x,
            socket_context->event_context.all_point_histories[i].end_point.y);
      }

      break;
    }
  }

  return EXIT_SUCCESS;
}
