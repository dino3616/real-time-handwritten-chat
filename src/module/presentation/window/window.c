#include "module/presentation/window/window.h"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

Display_t *open_display() { return XOpenDisplay(NULL); }

void close_display(Display_t *display) { XCloseDisplay(display); }

WindowManager_t create_window(int x, int y, int width, int height, char *title,
                              Display_t *display) {
  Window window =
      XCreateSimpleWindow(display, RootWindow(display, 0), x, y, width, height,
                          0, BlackPixel(display, 0), WhitePixel(display, 0));
  XStoreName(display, window, title);
  XSelectInput(
      display, window,
      ExposureMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask);

  GC gc = XCreateGC(display, window, None, NULL);
  XSetForeground(display, gc, 0);

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

  return (WindowManager_t){display, window, gc};
}

void destroy_window(WindowManager_t *window_manager) {
  XDestroyWindow(window_manager->display, window_manager->window);
}
