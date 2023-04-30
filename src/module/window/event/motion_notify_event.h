#ifndef MODULE_EVENT_MOTION_NOTIFY_EVENT_H
#define MODULE_EVENT_MOTION_NOTIFY_EVENT_H

#include <X11/Xlib.h>

#include "module/window/window.h"

extern void motion_notify_event(XPoint *startPoint, XEvent *event,
                                WindowManager_t *window_manager);

#endif
