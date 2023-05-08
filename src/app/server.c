#include "app/server.h"

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
#include "module/error/error.h"
#include "module/socket/socket.h"

int launch_server() {
  char host_name[HOST_NAME_SIZE];
  if (gethostname(host_name, sizeof(host_name)) < 0) {
    int error_number = errno;
    print_error("Failed to get host name. cause: '%s'\n",
                strerror(error_number));

    return EXIT_FAILURE;
  }
  printf("Your host name is '%s'\n", host_name);
  if (setenv("HOST_NAME", host_name, 1) != EXIT_SUCCESS) {
    int error_number = errno;
    print_error(
        "Failed to set host name in environment variable. cause: '%s'\n",
        strerror(error_number));

    return EXIT_FAILURE;
  }

  int socket_fd;
  if (listen_with_socket(host_name, PORT, &socket_fd) == NULL) {
    print_error("Failed to listen with socket.");

    return EXIT_FAILURE;
  }

  int client_count = 0;
  int client_fds[CLIENT_MAX];
  bzero(client_fds, sizeof(int) * CLIENT_MAX);

  bool has_next;

  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 1;

  while (true) {
    fd_set mask;
    FD_ZERO(&mask);
    FD_SET(socket_fd, &mask);
    FD_SET(0, &mask);

    has_next = true;
    int fd_max = socket_fd;
    for (int i = 0; i < CLIENT_MAX; i++) {
      int client_fd = client_fds[i];

      if (client_fd > 0) {
        FD_SET(client_fd, &mask);
      }
      if (client_fd > fd_max) {
        fd_max = client_fd;
      }
    }

    int result = select(fd_max + 1, &mask, NULL, NULL, &timeout);
    if (result < 0) {
      int error_number = errno;
      print_error("Failed to observe file descriptors. cause: '%s'\n",
                  strerror(error_number));

      return EXIT_FAILURE;
    }

    if (FD_ISSET(socket_fd, &mask)) {
      int new_client_fd = accept(socket_fd, NULL, NULL);
      if (new_client_fd < 0) {
        int error_number = errno;
        print_error("Failed to accept new client. cause: '%s'\n",
                    strerror(error_number));

        return EXIT_FAILURE;
      }

      for (int i = 0; i < CLIENT_MAX; i++) {
        if (client_fds[i] == 0) {
          client_fds[i] = new_client_fd;
          client_count++;

          printf("Client[%02d] created.\n", i);

          break;
        }
      }
    }

    for (int i = 0; i < CLIENT_MAX; i++) {
      int fd = client_fds[i];
      if (fd == 0) {
        continue;
      }

      if (FD_ISSET(fd, &mask)) {
        char recieved_buf[RECV_BUF_SIZE];
        bzero(recieved_buf, sizeof(char) * RECV_BUF_SIZE);
        int recieved_buf_length = read(fd, recieved_buf, RECV_BUF_SIZE);
        if (strcmp(recieved_buf, "q") == 0 || recieved_buf_length == 0) {
          close(fd);
          client_fds[i] = 0;
          client_count--;

          printf("Client[%02d] destroyed.\n", i);

          continue;
        }

        //   bzero(strBuf, sizeof(char) * BUF_SIZE_LONG);
        //   sprintf(strBuf, "client[%02d]: %s", i, buf);
        //   // Print the received message (fd=1 means standard output)
        //   write(1, strBuf, strlen(strBuf));
        //   // And also send that message to all other clients,
        //   // except for the client who sent the message just now
        //   for (int j = 0; j < CLIENT_MAX; j++) {
        //     int tmp = client_fds[j];
        //     if (j == i || tmp == 0) continue;
        //     write(tmp, strBuf, strlen(strBuf));
        //   }
      }
    }
    if (!has_next) break;
  }

  for (int i = 0; i < CLIENT_MAX; i++) {
    int client_fd = client_fds[i];
    if (client_fd > 0) {
      close(client_fd);
    }
  }

  close(socket_fd);

  return EXIT_SUCCESS;
}
