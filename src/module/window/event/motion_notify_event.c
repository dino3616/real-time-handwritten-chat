#include "module/window/event/motion_notify_event.h"

#include <X11/Xlib.h>

#include "module/window/window.h"

void motion_notify_event(XPoint *startPoint, XEvent *event,
                         WindowManager_t *window_manager) {
  XDrawLine(window_manager->display, window_manager->window, window_manager->gc,
            startPoint->x, startPoint->y, event->xmotion.x, event->xmotion.y);

  startPoint->x = event->xmotion.x;
  startPoint->y = event->xmotion.y;
}
