#include "app/server/server.h"

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

int launch_server() {
  char host_name[HOST_NAME_SIZE];
  if (gethostname(host_name, sizeof(host_name)) < 0) {
    int error_number = errno;
    log_error("Failed to get host name. cause: '%s'", strerror(error_number));

    return EXIT_FAILURE;
  }
  log_info("Your host name is '%s'", host_name);
  if (setenv("HOST_NAME", host_name, 1) != EXIT_SUCCESS) {
    int error_number = errno;
    log_error("Failed to set host name in environment variable. cause: '%s'",
              strerror(error_number));

    return EXIT_FAILURE;
  }

  int socket_fd;
  if (listen_with_socket(host_name, PORT, &socket_fd) == NULL) {
    log_error("Failed to listen with socket.");

    return EXIT_FAILURE;
  }

  int *client_fds = (int *)calloc(CLIENT_FDS_ALLOCATION_CHUNK, sizeof(int));
  if (client_fds == NULL) {
    int error_number = errno;
    log_error("Failed to allocate memory for client fds. cause: '%s'",
              strerror(error_number));

    return EXIT_FAILURE;
  }
  int client_fds_length = CLIENT_FDS_ALLOCATION_CHUNK;
  int client_fd_count = 0;

  struct timeval timeout = {.tv_sec = 0, .tv_usec = 1};

  PaintHistory_t all_paint_histories[PAINT_HISTORY_SIZE];
  bzero(all_paint_histories, sizeof(PaintHistory_t) * PAINT_HISTORY_SIZE);

  while (true) {
    fd_set mask;
    FD_ZERO(&mask);
    FD_SET(socket_fd, &mask);
    FD_SET(0, &mask);

    int fd_max = socket_fd;
    for (int i = 0; i < client_fds_length; i++) {
      if (client_fds[i] > 0) {
        FD_SET(client_fds[i], &mask);
      }
      if (client_fds[i] > fd_max) {
        fd_max = client_fds[i];
      }
    }

    int result = select(fd_max + 1, &mask, NULL, NULL, &timeout);
    if (result < 0) {
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

      if (command == 'q') {
        printf("Bye-bye :)\n");

        break;
      }
    }

    if (FD_ISSET(socket_fd, &mask)) {
      int new_client_fd = accept(socket_fd, NULL, NULL);
      if (new_client_fd < 0) {
        int error_number = errno;
        log_error("Failed to accept new client. cause: '%s'",
                  strerror(error_number));

        return EXIT_FAILURE;
      }

      for (int i = 0; i < client_fds_length; i++) {
        if (client_fds[i] == 0) {
          client_fds[i] = new_client_fd;
          client_fd_count++;

          log_info("client[%d] created.", i);

          if (client_fd_count == client_fds_length) {
            int *new_client_fds = (int *)realloc(
                client_fds, (client_fds_length + CLIENT_FDS_ALLOCATION_CHUNK) *
                                sizeof(int));
            if (new_client_fds == NULL) {
              int error_number = errno;
              log_error(
                  "Failed to reallocate memory for client fds. cause: '%s'",
                  strerror(error_number));

              return EXIT_FAILURE;
            }

            client_fds = new_client_fds;
            client_fds_length += CLIENT_FDS_ALLOCATION_CHUNK;
          }

          break;
        }
      }
    }

    for (int i = 0; i < client_fds_length; i++) {
      if (client_fds[i] == 0) {
        continue;
      }

      if (FD_ISSET(client_fds[i], &mask)) {
        SocketContext_t socket_context;
        if (recv(client_fds[i], &socket_context, sizeof(SocketContext_t), 0) <
            0) {
          int error_number = errno;
          log_error(
              "Failed to receive socket context from client[%d]. cause: '%s'",
              i, strerror(error_number));

          return EXIT_FAILURE;
        }

        switch (socket_context.command_context.command) {
          case 'q': {
            close(client_fds[i]);
            client_fds[i] = 0;
            client_fd_count--;

            log_info("client[%d] destroyed.", i);

            continue;
          }
        }

        switch (socket_context.event_context.event_type) {
          case PAINTED_EVENT: {
            for (int j = 0; j < PAINT_HISTORY_SIZE; j++) {
              if (all_paint_histories[j].start_point.x == 0 &&
                  all_paint_histories[j].start_point.y == 0 &&
                  all_paint_histories[j].end_point.x == 0 &&
                  all_paint_histories[j].end_point.y == 0) {
                all_paint_histories[j] =
                    socket_context.event_context.additional_paint_history;

                break;
              }
            }

            for (int j = 0; j < client_fds_length; j++) {
              if (j == i || client_fds[j] == 0) {
                continue;
              }

              if (send(client_fds[j], &socket_context, sizeof(SocketContext_t),
                       0) < 0) {
                int error_number = errno;
                log_error("Failed to send socket context. cause: '%s'",
                          strerror(error_number));

                return EXIT_FAILURE;
              }
            }

            break;
          }
          case EXPOSE_EVENT: {
            memcpy(socket_context.event_context.all_paint_histories,
                   all_paint_histories,
                   sizeof(PaintHistory_t) * PAINT_HISTORY_SIZE);

            if (send(client_fds[i], &socket_context, sizeof(SocketContext_t),
                     0) < 0) {
              int error_number = errno;
              log_error("Failed to send socket context. cause: '%s'",
                        strerror(error_number));

              return EXIT_FAILURE;
            }

            break;
          }
        }
      }
    }
  }

  for (int i = 0; i < client_fds_length; i++) {
    if (client_fds[i] > 0) {
      close(client_fds[i]);
    }
  }
  close(socket_fd);

  free(client_fds);

  return EXIT_SUCCESS;
}
