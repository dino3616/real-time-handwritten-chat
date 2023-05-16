#include "app/client/client.h"

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
#include "core/context/context.h"
#include "core/log/log.h"
#include "module/infra/socket/socket.h"
#include "module/presentation/command/command.h"
#include "module/presentation/command/help_command.h"
#include "module/presentation/window/event.h"
#include "module/presentation/window/window.h"

int launch_client() {
  WindowManager_t window_manager;
  if (create_window(500, 200, 600, 400, "real-time-handwritten-chat",
                    open_display(), &window_manager) == NULL) {
    log_error("Failed to create window.");

    return EXIT_FAILURE;
  }

  char* host_name = getenv("HOST_NAME");
  if (host_name == NULL) {
    host_name = malloc(sizeof(char) * HOST_NAME_SIZE);
    if (host_name == NULL) {
      int error_number = errno;
      log_error("Failed to allocate memory for host name. cause: '%s'",
                strerror(error_number));

      return EXIT_FAILURE;
    }

    printf("Enter the host name of server to connect to: ");
    if (fgets(host_name, HOST_NAME_SIZE, stdin) == NULL) {
      int error_number = errno;
      log_error("Failed to read host name. cause: '%s'",
                strerror(error_number));

      return EXIT_FAILURE;
    }
    if (host_name[strlen(host_name) - 1] == '\n') {
      host_name[strlen(host_name) - 1] = '\0';
    }
  }

  int socket_fd;
  if (connect_with_socket(host_name, PORT, &socket_fd) == NULL) {
    log_error("Failed to connect to server.");

    return EXIT_FAILURE;
  }

  free(host_name);

  if (help_command() != EXIT_SUCCESS) {
    log_error("Failed to execute help command.");

    return EXIT_FAILURE;
  }
  printf("> ");
  fflush(stdout);

  struct timeval timeout = {.tv_sec = 0, .tv_usec = 1};

  while (true) {
    fd_set mask;
    FD_ZERO(&mask);
    FD_SET(STDIN_FILENO, &mask);
    FD_SET(socket_fd, &mask);

    SocketContext_t socket_context;
    bzero(&socket_context, sizeof(SocketContext_t));

    render_event_handler(&window_manager, socket_fd, &socket_context);

    if (select(socket_fd + 1, &mask, NULL, NULL, &timeout) < 0) {
      int error_number = errno;
      log_error("Failed to observe file descriptors. cause: '%s'",
                strerror(error_number));

      return EXIT_FAILURE;
    }

    if (FD_ISSET(STDIN_FILENO, &mask)) {
      char command;
      if (read(STDIN_FILENO, &command, sizeof(char)) < 0) {
        int error_number = errno;
        log_error("Failed to read from stdin. cause: '%s'",
                  strerror(error_number));

        return EXIT_FAILURE;
      }
      while (getchar() != '\n') {
      }

      socket_context.command_context.command = command;

      if (parse_command(command, &window_manager, socket_fd, &socket_context) !=
          EXIT_SUCCESS) {
        log_error("Something went wrong while processing command.");
      }

      printf("> ");
      fflush(stdout);
    }

    if (FD_ISSET(socket_fd, &mask)) {
      if (recv(socket_fd, &socket_context, sizeof(SocketContext_t), 0) < 0) {
        int error_number = errno;
        log_error("Failed to receive socket context from server. cause: '%s'",
                  strerror(error_number));

        return EXIT_FAILURE;
      }

      if (rerender_event_handler(&window_manager, &socket_context) !=
          EXIT_SUCCESS) {
        log_error("Failed to rerender window.");
      }
    }
  }

  close_socket(socket_fd);

  destroy_window(&window_manager);
  close_display(window_manager.display);

  return EXIT_SUCCESS;
}
