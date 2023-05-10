#include "module/presentation/command/quit_command.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/log/log.h"

int quit_command() {
  printf("Bye-bye :)\n");

  exit(EXIT_SUCCESS);

  int error_number = errno;
  log_error("Failed to exit the program. cause: '%s'", strerror(error_number));

  return EXIT_FAILURE;
}
