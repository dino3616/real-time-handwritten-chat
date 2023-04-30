#include "module/command/command.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "module/command/color_command.h"
#include "module/command/help_command.h"
#include "module/command/quit_command.h"
#include "module/error/error.h"
#include "module/util/color.h"
#include "module/window/window.h"

int parse_command(char *input, WindowManager_t *window_manager) {
  char command = input[0];

  switch (command) {
    case 'c':
      if (color_command(window_manager) != EXIT_SUCCESS) {
        print_error("Failed to execute color command.");

        return EXIT_FAILURE;
      }

      break;
    case 'q':
      if (quit_command() != EXIT_SUCCESS) {
        print_error("Failed to execute quit command.");

        return EXIT_FAILURE;
      }

      break;
    case 'h':
      if (help_command() != EXIT_SUCCESS) {
        print_error("Failed to execute help command.");

        return EXIT_FAILURE;
      }

      break;
    default:
      print_error("Invalid command input. Expected: [c | q], got '%c'.",
                  command);

      break;
  }

  return EXIT_SUCCESS;
}
