#include "module/presentation/command/message_command.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "core/context/context.h"
#include "core/log/log.h"

int message_command(WindowManager_t* window_manager, int socket_fd,
                    SocketContext_t* socket_context) {
  socket_context->request_method = GET_REQUEST_METHOD;
  if (send(socket_fd, socket_context, sizeof(SocketContext_t), 0) < 0) {
    int error_number = errno;
    log_error("Failed to send socket context. cause: '%s'",
              strerror(error_number));

    return EXIT_FAILURE;
  }
  if (recv(socket_fd, socket_context, sizeof(SocketContext_t), 0) < 0) {
    int error_number = errno;
    log_error("Failed to receive socket context. cause: '%s'",
              strerror(error_number));

    return EXIT_FAILURE;
  }

  printf("========== Message History ==========\n");
  for (int i = 0; i < MESSAGE_HISTORY_SIZE; i++) {
    if (strlen(socket_context->command_context.message_context
                   .all_message_histories[i]
                   .message) == 0) {
      break;
    }

    printf(
        "\33[38;2;%d;%d;%dm",
        socket_context->command_context.message_context.all_message_histories[i]
            .rgb_color.r,
        socket_context->command_context.message_context.all_message_histories[i]
            .rgb_color.g,
        socket_context->command_context.message_context.all_message_histories[i]
            .rgb_color.b);
    printf(
        "%d: %s\n",
        socket_context->command_context.message_context.all_message_histories[i]
            .client_id,
        socket_context->command_context.message_context.all_message_histories[i]
            .message);
    printf("\33[0m");
  }
  printf("=====================================\n");

  char message[MESSAGE_LENGTH];
  printf("Enter a message to send to other clients (max 255 characters).\n");
  printf("[message cmd] > ");
  fflush(stdout);
  if (fgets(message, sizeof(message), stdin) == NULL) {
    int error_number = errno;
    log_error("Failed to read color input. cause: '%s'",
              strerror(error_number));

    return EXIT_FAILURE;
  }
  if (message[strlen(message) - 1] == '\n') {
    message[strlen(message) - 1] = '\0';
  }

  socket_context->command_context.message_context.additional_message_history
      .rgb_color = window_manager->foreground_rgb_color;
  strcpy(socket_context->command_context.message_context
             .additional_message_history.message,
         message);

  socket_context->request_method = POST_REQUEST_METHOD;
  if (send(socket_fd, socket_context, sizeof(SocketContext_t), 0) < 0) {
    int error_number = errno;
    log_error("Failed to send socket context. cause: '%s'",
              strerror(error_number));

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
