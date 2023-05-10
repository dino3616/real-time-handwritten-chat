#include "module/presentation/window/event.h"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>

#include "core/context/context.h"
#include "core/log/log.h"
#include "module/presentation/window/color.h"
#include "module/presentation/window/window.h"

int render_event_handler(WindowManager_t *window_manager, int socket_fd,
                         SocketContext_t *socket_context) {
  static XEvent event;
  static XPoint startPoint = {.x = 0, .y = 0};
  static PaintHistory_t paint_histories[PAINT_HISTORY_SIZE];

  if (XEventsQueued(window_manager->display, QueuedAfterFlush) == None) {
    return EXIT_SUCCESS;
  };

  XNextEvent(window_manager->display, &event);

  switch (event.type) {
    case ButtonPress: {
      bzero(paint_histories, sizeof(PaintHistory_t) * PAINT_HISTORY_SIZE);

      startPoint.x = event.xbutton.x;
      startPoint.y = event.xbutton.y;

      break;
    }
    case ButtonRelease: {
      socket_context->event_context.event_type = PAINTED_EVENT;
      if (memcpy(socket_context->event_context.additional_paint_histories,
                 paint_histories,
                 sizeof(PaintHistory_t) * PAINT_HISTORY_SIZE) == NULL) {
        int error_number = errno;
        log_error("Failed to copy point histories. cause: '%s'",
                  strerror(error_number));

        return EXIT_FAILURE;
      }

      send(socket_fd, socket_context, sizeof(SocketContext_t), 0);

      bzero(paint_histories, sizeof(PaintHistory_t) * PAINT_HISTORY_SIZE);
      bzero(socket_context->event_context.additional_paint_histories,
            sizeof(PaintHistory_t) * PAINT_HISTORY_SIZE);

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

      for (int i = 0; i < PAINT_HISTORY_SIZE; i++) {
        if (paint_histories[i].start_point.x == 0 &&
            paint_histories[i].start_point.y == 0 &&
            paint_histories[i].end_point.x == 0 &&
            paint_histories[i].end_point.y == 0) {
          paint_histories[i].rgb_color = window_manager->foreground_rgb_color;
          paint_histories[i].start_point.x = startPoint.x;
          paint_histories[i].start_point.y = startPoint.y;
          paint_histories[i].end_point.x = event.xmotion.x;
          paint_histories[i].end_point.y = event.xmotion.y;

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
      RGBColor_t own_rgb_color = window_manager->foreground_rgb_color;

      for (int i = 0; i < PAINT_HISTORY_SIZE; i++) {
        if (socket_context->event_context.additional_paint_histories[i]
                    .start_point.x == 0 &&
            socket_context->event_context.additional_paint_histories[i]
                    .start_point.y == 0 &&
            socket_context->event_context.additional_paint_histories[i]
                    .end_point.x == 0 &&
            socket_context->event_context.additional_paint_histories[i]
                    .end_point.y == 0) {
          break;
        }

        change_foreground_color(
            &socket_context->event_context.additional_paint_histories[i]
                 .rgb_color,
            window_manager);

        XDrawLine(window_manager->display, window_manager->window,
                  window_manager->gc,
                  socket_context->event_context.additional_paint_histories[i]
                      .start_point.x,
                  socket_context->event_context.additional_paint_histories[i]
                      .start_point.y,
                  socket_context->event_context.additional_paint_histories[i]
                      .end_point.x,
                  socket_context->event_context.additional_paint_histories[i]
                      .end_point.y);
      }

      change_foreground_color(&own_rgb_color, window_manager);

      break;
    }
    case EXPOSE_EVENT: {
      RGBColor_t own_rgb_color = window_manager->foreground_rgb_color;

      for (int i = 0; i < PAINT_HISTORY_SIZE; i++) {
        if (socket_context->event_context.all_paint_histories[i]
                    .start_point.x == 0 &&
            socket_context->event_context.all_paint_histories[i]
                    .start_point.y == 0 &&
            socket_context->event_context.all_paint_histories[i].end_point.x ==
                0 &&
            socket_context->event_context.all_paint_histories[i].end_point.y ==
                0) {
          break;
        }

        change_foreground_color(
            &socket_context->event_context.all_paint_histories[i].rgb_color,
            window_manager);

        XDrawLine(
            window_manager->display, window_manager->window, window_manager->gc,
            socket_context->event_context.all_paint_histories[i].start_point.x,
            socket_context->event_context.all_paint_histories[i].start_point.y,
            socket_context->event_context.all_paint_histories[i].end_point.x,
            socket_context->event_context.all_paint_histories[i].end_point.y);
      }

      change_foreground_color(&own_rgb_color, window_manager);

      break;
    }
  }

  return EXIT_SUCCESS;
}
