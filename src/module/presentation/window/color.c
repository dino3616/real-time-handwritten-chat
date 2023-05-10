#include "module/presentation/window/color.h"

#include <X11/Xlib.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/log/log.h"
#include "core/util/color.h"
#include "module/presentation/window/window.h"

int change_foreground_color(RGBColor_t *rgb_color,
                            WindowManager_t *window_manager) {
  char color_buf[16];
  if (sprintf(color_buf, "rgb:%02x/%02x/%02x", rgb_color->r, rgb_color->g,
              rgb_color->b) < 0) {
    int error_number = errno;
    log_error("Failed to format color. cause: '%s'", strerror(error_number));

    return EXIT_FAILURE;
  }

  XColor screen_color, exact_color;
  XAllocNamedColor(window_manager->display,
                   DefaultColormap(window_manager->display,
                                   DefaultScreen(window_manager->display)),
                   color_buf, &screen_color, &exact_color);

  XSetForeground(window_manager->display, window_manager->gc,
                 screen_color.pixel);

  return EXIT_SUCCESS;
}
