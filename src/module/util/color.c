#include "module/util/color.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "module/error/error.h"

RGBColor_t *parse_color(char *color_hex, RGBColor_t *rgb_color,
                        unsigned long *pixel) {
  if (sscanf(color_hex, "%lx", pixel) != 1) {
    int error_number = errno;
    print_error(
        "Failed to parse color. Expected hex format, got '%s'. "
        "cause: ",
        color_hex, strerror(error_number));

    return NULL;
  }

  if (strlen(color_hex) == 3) {
    sscanf(color_hex, "%1x%1x%1x", &rgb_color->r, &rgb_color->g, &rgb_color->b);

    rgb_color->r += rgb_color->r * 16;
    rgb_color->g += rgb_color->g * 16;
    rgb_color->b += rgb_color->b * 16;
  } else if (strlen(color_hex) == 6) {
    sscanf(color_hex, "%02x%02x%02x", &rgb_color->r, &rgb_color->g,
           &rgb_color->b);
  } else {
    print_error(
        "Invalid color code format. Expected 3 or 6 characters, got '%s'.",
        color_hex);

    return NULL;
  }

  return rgb_color;
}
