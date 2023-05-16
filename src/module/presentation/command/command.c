#include "module/presentation/command/command.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "core/log/log.h"
#include "core/util/color.h"
#include "module/presentation/command/color_command.h"
#include "module/presentation/command/help_command.h"
#include "module/presentation/command/message_command.h"
#include "module/presentation/command/quit_command.h"
#include "module/presentation/window/window.h"

int parse_command(char command, WindowManager_t *window_manager, int socket_fd,
                  SocketContext_t *socket_context) {
  switch (command) {
    case 'c': {
      if (color_command(window_manager) != EXIT_SUCCESS) {
        log_error("Failed to execute color command.");

        return EXIT_FAILURE;
      }

      break;
    }
    case 'h': {
      if (help_command() != EXIT_SUCCESS) {
        log_error("Failed to execute help command.");

        return EXIT_FAILURE;
      }

      break;
    }
    case 'm': {
      if (message_command(window_manager, socket_fd, socket_context) !=
          EXIT_SUCCESS) {
        log_error("Failed to execute move command.");

        return EXIT_FAILURE;
      }

      break;
    }
    case 'q': {
      if (quit_command(socket_fd, socket_context) != EXIT_SUCCESS) {
        log_error("Failed to execute quit command.");

        return EXIT_FAILURE;
      }

      break;
    }
    default: {
      log_error("Invalid command input. Expected: [c | h | m | q], got '%c'.",
                command);

      break;
    }
  }

  return EXIT_SUCCESS;
}
