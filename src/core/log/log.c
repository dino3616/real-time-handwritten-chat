#include "core/log/log.h"

#include <stdarg.h>
#include <stdio.h>

void log_error(const char *format, ...) {
  va_list args;

  va_start(args, format);
  fprintf(stderr, "\033[31m");
  fprintf(stderr, "Error: ");
  vfprintf(stderr, format, args);
  fprintf(stderr, "\033[0m\n");
  va_end(args);

  return;
}
