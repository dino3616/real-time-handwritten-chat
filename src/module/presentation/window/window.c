#include "module/presentation/window/window.h"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "core/log/log.h"

Display_t *open_display() { return XOpenDisplay(NULL); }

void close_display(Display_t *display) { XCloseDisplay(display); }

WindowManager_t *create_window(int x, int y, int width, int height, char *title,
                               Display_t *display,
                               WindowManager_t *window_manager) {
  Window window =
      XCreateSimpleWindow(display, RootWindow(display, 0), x, y, width, height,
                          0, BlackPixel(display, 0), WhitePixel(display, 0));
  XStoreName(display, window, title);
  XSelectInput(
      display, window,
      ExposureMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask);

  GC gc = XCreateGC(display, window, None, NULL);

  char color_hex[7];
  srand((unsigned int)time(NULL));
  if (sprintf(color_hex, "%06x", rand() % 0xFFFFFF) < 0) {
    log_error("Failed to generate random color hex.");

    return NULL;
  }

  RGBColor_t foreground_rgb_color;
  unsigned long foreground_pixel;
  if (parse_color(color_hex, &foreground_rgb_color, &foreground_pixel) ==
      NULL) {
    log_error("Failed to parse color.");

    return NULL;
  }
  XSetForeground(display, gc, foreground_pixel);

  printf("\33[38;2;%d;%d;%dm", foreground_rgb_color.r, foreground_rgb_color.g,
         foreground_rgb_color.b);
  printf("Your pen color is #%s.\n", color_hex);
  printf("\33[48;2;%d;%d;%dm", foreground_rgb_color.r, foreground_rgb_color.g,
         foreground_rgb_color.b);
  printf("        ");
  printf("\33[0m\n");

  XSetWindowAttributes attributes;
  attributes.backing_store = WhenMapped;
  XChangeWindowAttributes(display, window, CWBackingStore, &attributes);
  XSetLineAttributes(display, gc, 4, LineSolid, CapRound, JoinRound);

  XSizeHints hints;
  hints.flags = PPosition | PSize;
  hints.x = x;
  hints.y = y;
  hints.width = width;
  hints.height = height;
  XSetNormalHints(display, window, &hints);

  XMapWindow(display, window);

  window_manager->display = display;
  window_manager->window = window;
  window_manager->gc = gc;
  window_manager->foreground_rgb_color = foreground_rgb_color;

  return window_manager;
}

void destroy_window(WindowManager_t *window_manager) {
  XDestroyWindow(window_manager->display, window_manager->window);
}
