#ifndef MODULE_COMMAND_COMMAND_H
#define MODULE_COMMAND_COMMAND_H

#include "module/window/window.h"

#define COMMAND_INPUT_BUF_SIZE 1024

extern int parse_command(char *input, WindowManager_t *window_manager);

#endif
