#include "module/command/quit_command.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "module/error/error.h"

int quit_command() {
  printf("Bye-bye :)\n");

  exit(EXIT_SUCCESS);

  int error_number = errno;
  print_error("Failed to exit the program. cause: '%s'\n",
              strerror(error_number));

  return EXIT_FAILURE;
}
