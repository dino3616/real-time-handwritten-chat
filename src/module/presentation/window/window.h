#ifndef MODULE_PRESENTATION_WINDOW_WINDOW_H
#define MODULE_PRESENTATION_WINDOW_WINDOW_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "core/util/color.h"

typedef Display Display_t;
typedef Window Window_t;

typedef struct WindowManager {
  Display_t *display;
  Window_t window;
  GC gc;
  RGBColor_t foreground_rgb_color;
} WindowManager_t;

extern Display_t *open_display();
extern void close_display(Display_t *display);

extern WindowManager_t create_window(int x, int y, int width, int height,
                                     char *title, Display_t *display);
extern void destroy_window(WindowManager_t *window_manager);

#endif
