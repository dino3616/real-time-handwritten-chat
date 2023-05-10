#ifndef MODULE_EVENT_EVENT_H
#define MODULE_EVENT_EVENT_H

#include "module/context/context.h"
#include "module/window/window.h"

extern int render_event_handler(WindowManager_t *window_manager, int socket_fd,
                                SocketContext_t *socket_context);

extern int rerender_event_handler(WindowManager_t *window_manager,
                                  SocketContext_t *socket_context);

#endif
