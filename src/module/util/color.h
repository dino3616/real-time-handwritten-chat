#ifndef MODULE_UTIL_COLOR_H
#define MODULE_UTIL_COLOR_H

typedef struct RGBColor {
  unsigned int r;
  unsigned int g;
  unsigned int b;
} RGBColor_t;

extern RGBColor_t *parse_color(char *color_hex, RGBColor_t *rgb_color,
                               unsigned long *pixel);

#endif
