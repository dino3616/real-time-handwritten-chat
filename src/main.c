#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "app/client/client.h"
#include "app/server/server.h"
#include "core/log/log.h"

int main(int argc, char *argv[]) {
  if (argc == 2) {
    if (strcmp(argv[1], "--client") == 0 || strcmp(argv[1], "--c") == 0) {
      log_info("The program has started in the client role! 🚀");

      if (launch_client() != EXIT_SUCCESS) {
        log_error("An error occured while running the client.");

        return EXIT_FAILURE;
      }
    } else if (strcmp(argv[1], "--server") == 0 ||
               strcmp(argv[1], "--s") == 0) {
      log_info("The program has started in the server role! 🛰️");

      if (launch_server() != EXIT_SUCCESS) {
        log_error("An error occured while running the server.");

        return EXIT_FAILURE;
      }
    } else {
      log_error("Invalid argument detected! 🛸\nRecieved: %s", argv[1]);

      return EXIT_FAILURE;
    }
  } else if (argc > 2) {
    log_error("Too many arguments detected! 🛸");

    return EXIT_FAILURE;
  } else {
    log_info("The program has started in the default (server) role! 🛰️");

    if (launch_server() != EXIT_SUCCESS) {
      log_error("An error occured while running the server.");

      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
