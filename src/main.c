#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/select.h>
#include <unistd.h>

#include "module/command/command.h"
#include "module/command/help_command.h"
#include "module/error/error.h"
#include "module/window/event/event.h"
#include "module/window/window.h"

int main() {
  WindowManager_t window_manager = create_window(
      500, 200, 600, 400, "real-time-handwritten-chat", open_display());

  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 1;

  if (help_command() != EXIT_SUCCESS) {
    print_error("Failed to execute help command.");

    return EXIT_FAILURE;
  }

  printf("> ");
  fflush(stdout);

  while (true) {
    event_handler(&window_manager);

    fd_set mask;
    FD_ZERO(&mask);
    FD_SET(STDIN_FILENO, &mask);

    int result = select(STDIN_FILENO + 1, &mask, NULL, NULL, &timeout);
    if (result < 0) {
      print_error("Failed to execute select().");

      return EXIT_FAILURE;
    }

    if (FD_ISSET(STDIN_FILENO, &mask)) {
      char input_buf[COMMAND_INPUT_BUF_SIZE];
      bzero(input_buf, sizeof(char) * COMMAND_INPUT_BUF_SIZE);

      int input_length =
          read(STDIN_FILENO, input_buf, sizeof(char) * COMMAND_INPUT_BUF_SIZE);
      if (input_length < 0) {
        print_error("Failed to execute read().");

        return EXIT_FAILURE;
      }

      if (parse_command(input_buf, &window_manager) != EXIT_SUCCESS) {
        print_error("Oops! Something went wrong while processing command :(");
      }

      printf("\n> ");
      fflush(stdout);
    }
  }

  destroy_window(&window_manager);
  close_display(window_manager.display);

  return EXIT_SUCCESS;
}
