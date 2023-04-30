#include "module/window/event/button_press_event.h"

#include <X11/Xlib.h>

void button_press_event_handler(XPoint *startPoint, XEvent *event) {
  startPoint->x = event->xbutton.x;
  startPoint->y = event->xbutton.y;
}
