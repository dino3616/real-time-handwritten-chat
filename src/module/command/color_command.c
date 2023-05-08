#include "module/command/color_command.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "module/error/error.h"
#include "module/util/color.h"
#include "module/window/color.h"
#include "module/window/window.h"

int color_command(WindowManager_t *window_manager) {
  char color_hex[8];
  printf("Enter color in hex format (e.g. ff0000).\n");
  printf("[color cmd] > ");
  fflush(stdout);
  if (fgets(color_hex, sizeof(color_hex), stdin) == NULL) {
    int error_number = errno;
    print_error("Failed to read color input. cause: '%s'\n",
                strerror(error_number));

    return EXIT_FAILURE;
  }
  if (color_hex[strlen(color_hex) - 1] == '\n') {
    color_hex[strlen(color_hex) - 1] = '\0';
  }

  RGBColor_t rgb_color;
  unsigned long pixel;
  if (parse_color(color_hex, &rgb_color, &pixel) == NULL) {
    print_error("Failed to parse color.");

    return EXIT_FAILURE;
  }

  change_foreground_color(&rgb_color, window_manager);

  printf("\33[38;2;%d;%d;%dm", rgb_color.r, rgb_color.g, rgb_color.b);
  printf("Successfully set the pen color to #%s.\n", color_hex);
  printf("\33[48;2;%d;%d;%dm", rgb_color.r, rgb_color.g, rgb_color.b);
  printf("        ");
  printf("\33[0m\n");

  return EXIT_SUCCESS;
}
