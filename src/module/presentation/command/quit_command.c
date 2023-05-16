#include "module/presentation/command/quit_command.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "core/context/context.h"
#include "core/log/log.h"

int quit_command(int socket_fd, SocketContext_t *socket_context) {
  printf("Bye-bye :)\n");

  socket_context->request_method = POST_REQUEST_METHOD;
  if (send(socket_fd, socket_context, sizeof(SocketContext_t), 0) < 0) {
    int error_number = errno;
    log_error("Failed to send socket context to server. cause: '%s'",
              strerror(error_number));

    return EXIT_FAILURE;
  }

  exit(EXIT_SUCCESS);

  int error_number = errno;
  log_error("Failed to exit the program. cause: '%s'", strerror(error_number));

  return EXIT_FAILURE;
}
