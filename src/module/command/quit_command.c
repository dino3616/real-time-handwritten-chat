#include "module/command/quit_command.h"

#include <stdio.h>
#include <stdlib.h>

#include "module/error/error.h"

int quit_command() {
  printf("Bye-bye :)\n");

  exit(EXIT_SUCCESS);

  print_error("Failed to execute exit().");

  return EXIT_FAILURE;
}
