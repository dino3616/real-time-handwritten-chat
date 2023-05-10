#ifndef MODULE_PRESENTATION_WINDOW_EVENT_H
#define MODULE_PRESENTATION_WINDOW_EVENT_H

#include "core/context/context.h"
#include "module/presentation/window/window.h"

extern int render_event_handler(WindowManager_t *window_manager, int socket_fd,
                                SocketContext_t *socket_context);

extern int rerender_event_handler(WindowManager_t *window_manager,
                                  SocketContext_t *socket_context);

#endif
