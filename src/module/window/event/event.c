#include "event.h"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>

#include "module/window/event/button_press_event.h"
#include "module/window/event/motion_notify_event.h"
#include "module/window/window.h"

void event_handler(WindowManager_t *window_manager) {
  static XEvent event;
  static XPoint startPoint = {0, 0};

  if (XEventsQueued(window_manager->display, QueuedAfterFlush) == None) {
    return;
  };

  XNextEvent(window_manager->display, &event);

  switch (event.type) {
    case ButtonPress:
      button_press_event_handler(&startPoint, &event);
      break;
    case MotionNotify:
      motion_notify_event(&startPoint, &event, window_manager);
      break;
  }
}
