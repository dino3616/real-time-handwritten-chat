#include "module/presentation/command/color_command.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/log/log.h"
#include "core/util/color.h"
#include "module/presentation/window/color.h"
#include "module/presentation/window/window.h"

int color_command(WindowManager_t *window_manager) {
  char color_hex[7];
  printf("Enter color in hex format (e.g. ff0000).\n");
  printf("[color cmd] > ");
  fflush(stdout);
  if (fgets(color_hex, sizeof(color_hex), stdin) == NULL) {
    int error_number = errno;
    log_error("Failed to read color input. cause: '%s'",
              strerror(error_number));

    return EXIT_FAILURE;
  }
  if (color_hex[strlen(color_hex) - 1] == '\n') {
    color_hex[strlen(color_hex) - 1] = '\0';
  }

  RGBColor_t rgb_color;
  unsigned long pixel;
  if (parse_color(color_hex, &rgb_color, &pixel) == NULL) {
    log_error("Failed to parse color.");

    return EXIT_FAILURE;
  }

  if (change_foreground_color(&rgb_color, window_manager) != EXIT_SUCCESS) {
    log_error("Failed to change the pen color.");

    return EXIT_FAILURE;
  }
  window_manager->foreground_rgb_color = rgb_color;

  printf("\33[38;2;%d;%d;%dm", rgb_color.r, rgb_color.g, rgb_color.b);
  printf("Successfully set the pen color to #%s.\n", color_hex);
  printf("\33[48;2;%d;%d;%dm", rgb_color.r, rgb_color.g, rgb_color.b);
  printf("        ");
  printf("\33[0m\n");

  return EXIT_SUCCESS;
}
