#ifndef MODULE_PRESENTATION_COMMAND_COMMAND_H
#define MODULE_PRESENTATION_COMMAND_COMMAND_H

#include "core/context/context.h"
#include "module/presentation/window/window.h"

extern int parse_command(char command, WindowManager_t *window_manager,
                         int socket_fd, SocketContext_t *socket_context);

#endif
