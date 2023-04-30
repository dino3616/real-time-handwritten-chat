#ifndef MODULE_EVENT_BUTTON_PRESS_EVENT_H
#define MODULE_EVENT_BUTTON_PRESS_EVENT_H

#include <X11/Xlib.h>

extern void button_press_event_handler(XPoint *startPoint, XEvent *event);

#endif
