#include "module/presentation/command/help_command.h"

#include <stdio.h>
#include <stdlib.h>

int help_command() {
  printf("Available commands:\n");
  printf("  c - Change the pen color.\n");
  printf("  h - Display this help message.\n");
  printf("  m - Send a message to other clients.\n");
  printf("  q - Quit the program.\n");

  return EXIT_SUCCESS;
}
