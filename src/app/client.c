#include "app/client.h"

#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#include "app/app.h"
#include "module/command/command.h"
#include "module/command/help_command.h"
#include "module/error/error.h"
#include "module/socket/socket.h"
#include "module/window/event/event.h"
#include "module/window/window.h"

int launch_client() {
  WindowManager_t window_manager = create_window(
      500, 200, 600, 400, "real-time-handwritten-chat", open_display());

  char* host_name = getenv("HOST_NAME");
  if (host_name == NULL) {
    host_name = malloc(sizeof(char) * HOST_NAME_SIZE);
    if (host_name == NULL) {
      int error_number = errno;
      print_error("Failed to allocate memory for host name. cause: '%s'\n",
                  strerror(error_number));

      return EXIT_FAILURE;
    }

    printf("Enter the host name of server to connect to: ");
    if (fgets(host_name, HOST_NAME_SIZE, stdin) == NULL) {
      int error_number = errno;
      print_error("Failed to read host name. cause: '%s'\n",
                  strerror(error_number));

      return EXIT_FAILURE;
    }
    if (host_name[strlen(host_name) - 1] == '\n') {
      host_name[strlen(host_name) - 1] = '\0';
    }
  }

  int socket_fd;
  if (connect_with_socket(host_name, PORT, &socket_fd) == NULL) {
    print_error("Failed to connect to server.");

    return EXIT_FAILURE;
  }

  free(host_name);

  if (help_command() != EXIT_SUCCESS) {
    print_error("Failed to execute help command.");

    return EXIT_FAILURE;
  }
  printf("> ");
  fflush(stdout);

  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 1;

  while (true) {
    event_handler(&window_manager);

    fd_set mask;
    FD_ZERO(&mask);
    FD_SET(STDIN_FILENO, &mask);
    FD_SET(socket_fd, &mask);

    int result = select(STDIN_FILENO + 1, &mask, NULL, NULL, &timeout);
    if (result < 0) {
      int error_number = errno;
      print_error("Failed to observe file descriptors. cause: '%s'\n",
                  strerror(error_number));

      return EXIT_FAILURE;
    }

    if (FD_ISSET(STDIN_FILENO, &mask)) {
      char input_buf[COMMAND_INPUT_BUF_SIZE];
      bzero(input_buf, sizeof(char) * COMMAND_INPUT_BUF_SIZE);

      int input_length =
          read(STDIN_FILENO, input_buf, sizeof(char) * COMMAND_INPUT_BUF_SIZE);
      if (input_length < 0) {
        int error_number = errno;
        print_error("Failed to read from stdin. cause: '%s'\n",
                    strerror(error_number));

        return EXIT_FAILURE;
      }

      if (parse_command(input_buf, &window_manager) != EXIT_SUCCESS) {
        print_error("Something went wrong while processing command.");
      }

      // write(socket_fd, inputBuf, inputLength);

      printf("\n> ");
      fflush(stdout);
    }

    // if (FD_ISSET(socket_fd, &mask)) {
    //   // bzero(buf, sizeof(char) * BUF_SIZE);
    //   // msgLength = read(socket_fd, buf, BUF_SIZE);
    //   // // "Quit" command received
    //   // if (strcmp(buf, "Q\n") == 0 || msgLength == 0) break;
    //   // // Print the received message (fd=1 means standard output)
    //   // write(1, buf, msgLength);
    // }
  }

  close_socket(socket_fd);

  destroy_window(&window_manager);
  close_display(window_manager.display);

  return EXIT_SUCCESS;
}
