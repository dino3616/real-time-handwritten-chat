#include "module/window/color.h"

#include <X11/Xlib.h>
#include <stdio.h>

#include "module/util/color.h"
#include "module/window/window.h"

void change_foreground_color(RGBColor_t *rgb_color,
                             WindowManager_t *window_manager) {
  char color_buf[16];
  sprintf(color_buf, "rgb:%02x/%02x/%02x", rgb_color->r, rgb_color->g,
          rgb_color->b);

  XColor screen_color, exact_color;
  XAllocNamedColor(window_manager->display,
                   DefaultColormap(window_manager->display,
                                   DefaultScreen(window_manager->display)),
                   color_buf, &screen_color, &exact_color);

  XSetForeground(window_manager->display, window_manager->gc,
                 screen_color.pixel);
}
