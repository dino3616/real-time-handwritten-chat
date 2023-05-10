#include "module/command/command.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "module/command/color_command.h"
#include "module/command/help_command.h"
#include "module/command/quit_command.h"
#include "module/error/error.h"
#include "module/util/color.h"
#include "module/window/window.h"

int parse_command(char command, WindowManager_t *window_manager, int socket_fd,
                  SocketContext_t socket_context) {
  switch (command) {
    case 'c': {
      if (color_command(window_manager) != EXIT_SUCCESS) {
        print_error("Failed to execute color command.");

        return EXIT_FAILURE;
      }

      break;
    }
    case 'q': {
      if (send(socket_fd, &socket_context, sizeof(SocketContext_t), 0) == -1) {
        int error_code = errno;
        print_error("Failed to send socket context to server. cause: '%s'",
                    strerror(error_code));

        return EXIT_FAILURE;
      }

      if (quit_command() != EXIT_SUCCESS) {
        print_error("Failed to execute quit command.");

        return EXIT_FAILURE;
      }

      break;
    }
    case 'h': {
      if (help_command() != EXIT_SUCCESS) {
        print_error("Failed to execute help command.");

        return EXIT_FAILURE;
      }

      break;
    }
    default: {
      print_error("Invalid command input. Expected: [c | q | h], got '%c'.",
                  command);

      break;
    }
  }

  return EXIT_SUCCESS;
}
