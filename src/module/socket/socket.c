#include "module/socket/socket.h"

#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <unistd.h>

#include "module/error/error.h"

int *create_socket(int port, char *host_name, int *socket_fd,
                   struct sockaddr_in *socket_addr);

int *connect_with_socket(char *host_name, int port, int *socket_fd) {
  struct sockaddr_in socket_addr;
  if (create_socket(port, host_name, socket_fd, &socket_addr) == NULL) {
    print_error("Failed to create socket.");

    return NULL;
  }

  if (connect(*socket_fd, (struct sockaddr *)&socket_addr,
              sizeof(socket_addr)) != EXIT_SUCCESS) {
    int error_number = errno;
    print_error("Failed to connect to server. cause: '%s'\n",
                strerror(error_number));

    return NULL;
  }

  return socket_fd;
}

int *listen_with_socket(char *host_name, int port, int *socket_fd) {
  struct sockaddr_in socket_addr;
  if (create_socket(port, host_name, socket_fd, &socket_addr) == NULL) {
    print_error("Failed to create socket.");

    return NULL;
  }

  if (bind(*socket_fd, (struct sockaddr *)&socket_addr, sizeof(socket_addr)) !=
      EXIT_SUCCESS) {
    int error_number = errno;
    print_error("Failed to bind socket. cause: '%s'\n", strerror(error_number));

    return NULL;
  }

  listen(*socket_fd, 1);

  return socket_fd;
}

int *create_socket(int port, char *host_name, int *socket_fd,
                   struct sockaddr_in *socket_addr) {
  struct hostent *host = gethostbyname(host_name);
  if (host == NULL) {
    int error_number = errno;
    print_error("Failed to get host by name. cause: '%s'\n",
                strerror(error_number));

    return NULL;
  }

  bzero((char *)socket_addr, sizeof(socket_addr));
  socket_addr->sin_family = AF_INET;
  socket_addr->sin_port = htons(port);
  bcopy((char *)host->h_addr_list[0], (char *)&socket_addr->sin_addr,
        host->h_length);

  *socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (*socket_fd < 0) {
    int error_number = errno;
    print_error("Failed to create socket. cause: '%s'\n",
                strerror(error_number));

    return NULL;
  }

  int option = 1;
  setsockopt(*socket_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

  return socket_fd;
}

void close_socket(int socket_fd) { close(socket_fd); }
